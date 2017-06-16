#include "THTexture.h"
#include "THAssetManager.h"
#include "THImage.h"
#include <Platform\THDevice.h>

namespace THEngine
{
	TextureImpl::TextureImpl()
	{
	}

	TextureImpl::~TextureImpl()
	{
		TH_SAFE_RELEASE(texture);
		TH_SAFE_RELEASE(texImage);
	}

	bool TextureImpl::SaveToFile(const String& path)
	{
		IDirect3DSurface9* surface;
		texture->GetSurfaceLevel(0, &surface);

		D3DXIMAGE_FILEFORMAT format;
		String ext = path.SubString(path.LastIndexOf(TCHAR('.')) + 1, path.GetLength());
		if (ext == "png")
		{
			format = D3DXIFF_PNG;
		}
		else if (ext == "jpg")
		{
			format = D3DXIFF_JPG;
		}
		else if (ext == "bmp")
		{
			format = D3DXIFF_BMP;
		}
		else
		{
			THMessageBox((String)"无法将纹理存储为文件。不支持的文件格式：" + ext);
			return false;
		}

		D3DXSaveSurfaceToFile(path.GetBuffer(), format, surface, nullptr, nullptr);
		return true;
	}

	void TextureImpl::OnLostDevice()
	{
		texImage = new Image(width, height);

		D3DLOCKED_RECT rect;
		texture->LockRect(0, &rect, nullptr, 0);

		unsigned char* src = (unsigned char*)rect.pBits;
		unsigned char* dest = (unsigned char*)texImage->GetData();
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				dest[j * 4] = src[j * 4 + 2];
				dest[j * 4 + 1] = src[j * 4 + 1];
				dest[j * 4 + 2] = src[j * 4];
				dest[j * 4 + 3] = src[j * 4 + 3];
			}
			dest += width * 4;
			src += rect.Pitch;
		}

		texture->UnlockRect(0);

		texImage->Retain();

		TH_SAFE_RELEASE(texture);
	}

	void TextureImpl::OnResetDevice()
	{
		auto device = Device::GetInstance()->GetD3DDevice();

		if (FAILED(D3DXCreateTexture(device, width, height, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture)))
		{
			ExceptionManager::GetInstance()->PushException(new Exception(
				((String)"纹理加载失败。原因是：\nD3DXCreateTexture失败。")));
			return;
		}

		D3DLOCKED_RECT rect;
		texture->LockRect(0, &rect, nullptr, 0);

		unsigned char* dest = (unsigned char*)rect.pBits;
		unsigned char* src = (unsigned char*)texImage->GetData();
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				dest[j * 4] = src[j * 4 + 2];
				dest[j * 4 + 1] = src[j * 4 + 1];
				dest[j * 4 + 2] = src[j * 4];
				dest[j * 4 + 3] = src[j * 4 + 3];
			}
			dest += rect.Pitch;
			src += width * 4;
		}

		texture->UnlockRect(0);

		TH_SAFE_RELEASE(texImage);
	}

	void TextureImpl::GenerateMipmap()
	{
		this->texture->GenerateMipSubLevels();
	}

	////////////////////////////////////////////
	Texture::Texture()
	{
	}

	Texture::~Texture()
	{
		auto assetManager = AssetManager::GetInstance();
		assetManager->DestroyTexture(this);
	}
}