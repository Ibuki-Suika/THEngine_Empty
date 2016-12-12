#include "THImage.h"
#include "../External/libjpeg/jpeglib.h"
#include "../External/libpng/png.h"
#include <stdlib.h>

#ifdef _DEBUG
#pragma comment(lib,"libjpeg/jpeg.lib")
#pragma comment(lib,"libpng/libpng16-debug.lib")
#pragma comment(lib,"libpng/zlib-debug.lib")
#else
#pragma comment(lib,"libjpeg/jpeg.lib")
#pragma comment(lib,"libpng/libpng16-release.lib")
#pragma comment(lib,"libpng/zlib-release.lib")
#endif

using namespace THEngine;

Image::Image()
{
	width = height = 0;
	data = nullptr;
}

Image::Image(int width, int height)
{
	this->width = width;
	this->height = height;

	data = new unsigned char[width * height * 4];
}

Image::~Image()
{
	if (data)
	{
		delete[] data;
	}
	data = nullptr;
}

Image* Image::Load(String filePath)
{
	auto exceptionManager = ExceptionManager::GetInstance();

	Image* image = new Image();
	String ext = filePath.SubString(filePath.LastIndexOf(TCHAR('.')) + 1, filePath.GetLength());

	if (ext == "jpg")
	{
		if (image->LoadJPG(filePath.ToStdString().c_str()) == false)
		{
			auto exception = exceptionManager->GetException();
			String message;
			if (exception)
			{
				message = (String)"读取图片失败。原因是:\n" + exception->GetInfo();
			}
			else
			{
				message = (String)"读取图片失败。原因是:\n" + "未知异常";
			}

			auto newException = new Exception();
			exceptionManager->PushException(newException);

			delete image;
			return nullptr;
		}
	}
	else if (ext == "png")
	{
		if (image->LoadPNG(filePath.ToStdString().c_str()) == false)
		{
			auto exception = exceptionManager->GetException();
			String message;
			if (exception)
			{
				message = (String)"读取图片失败。原因是:\n" + exception->GetInfo();
			}
			else
			{
				message = (String)"读取图片失败。原因是:\n" + "未知异常";
			}

			auto newException = new Exception();
			exceptionManager->PushException(newException);

			delete image;
			return nullptr;
		}
	}
	else
	{
		Exception* exception = new Exception("不支持的文件格式。");
		exceptionManager->PushException(exception);
		delete image;
		return nullptr;
	}
	return image;
}

bool Image::LoadJPG(const char* filePath)
{
	auto exceptionManager = ExceptionManager::GetInstance();

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr error;

	FILE* infile;
	if ((infile = fopen(filePath, "rb")) == NULL)
	{
		exceptionManager->PushException(new Exception("无法打开文件。"));
		return false;
	}

	cinfo.err = jpeg_std_error(&error);

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);

	jpeg_start_decompress(&cinfo);

	width = cinfo.output_width;
	height = cinfo.output_height;

	data = new unsigned char[width * height * 4];

	int row_stride = cinfo.output_width * 4;

	JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
	int i = 0;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				data[i * row_stride + j * 4 + k] = buffer[0][j * 3 + k];
			}
			data[i * row_stride + j * 4 + 3] = 255;
		}
		i++;
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	return true;
}

bool Image::LoadPNG(const char* filePath)
{
	auto exceptionManager = ExceptionManager::GetInstance();

	png_structp png_ptr;
	png_infop info_ptr;
	FILE* infile;

	if ((infile = fopen(filePath, "rb")) == NULL)
	{
		exceptionManager->PushException(new Exception("无法打开文件。"));
		return false;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	info_ptr = png_create_info_struct(png_ptr);

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		/* Free all of the memory associated with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(infile);
		/* If we get here, we had a problem reading the file */
		exceptionManager->PushException(new Exception("无法读取文件内容。文件可能已损坏。"));
		return false;
	}

	png_init_io(png_ptr, infile);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	auto color_type = png_get_color_type(png_ptr, info_ptr);
	auto row_ptr = png_get_rows(png_ptr, info_ptr);

	data = new unsigned char[width * height * 4];

	switch (color_type)
	{
	case PNG_COLOR_TYPE_RGBA:
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					data[i * width * 4 + j * 4 + k] = row_ptr[i][j * 4 + k];
				}
			}
		}
		break;
	case PNG_COLOR_TYPE_RGB:
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					data[i * width * 4 + j * 4 + k] = row_ptr[i][j * 3 + k];
				}
				data[i * width * 4 + j * 4 + 3] = 255;
			}
		}
		break;
	default:
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		fclose(infile);
		return false;
	}
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	fclose(infile);

	return true;
}