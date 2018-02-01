#include "THAssetManager.h"
#include "THImage.h"
#include "THRenderTexture.h"
#include "THFloatTexture.h"
#include <Core\THGame.h>
#include <Platform\THDevice.h>

namespace THEngine
{
	bool AssetManager::Init()
	{
		return true;
	}

	Ptr<Shader> AssetManager::CreateShaderFromFile(const String& filePath)
	{
		Ptr<Shader> shader = Ptr<Shader>::New();
		auto dd = Device::GetInstance();
		auto device = Device::GetInstance()->GetD3DDevice();

		ID3DXBuffer *error;
		HRESULT hr;
		if (FAILED(hr = D3DXCreateEffectFromFile(device, filePath.GetBuffer(), NULL, NULL,
			D3DXSHADER_DEBUG, NULL, &shader->effect, &error)))
		{
			if (error)
			{
				String message((char*)error->GetBufferPointer());
				THMessageBox(message);
			}
			else
			{
				THMessageBox((String)"无法打开文件:" + filePath);
			}
			return nullptr;
		}

		shader->path = filePath;
		shaderList.Add(shader);
		return shader;
	}

	inline void AssetManager::DestroyShader(Ptr<Shader> shader)
	{
		shaderList.Remove(shader);
	}

	void AssetManager::CopyImageToSurface(Ptr<Image> image, IDirect3DSurface9* surface)
	{
		D3DLOCKED_RECT rect;
		RECT imageRect;
		imageRect.left = imageRect.top = 0;
		imageRect.right = image->GetWidth();
		imageRect.bottom = image->GetHeight();
		HRESULT hr = surface->LockRect(&rect, &imageRect, 0);
		unsigned char* dest = (unsigned char*)rect.pBits;
		unsigned char* src = (unsigned char*)image->GetData();
		for (int i = 0; i < image->GetHeight(); i++)
		{
			for (int j = 0; j < image->GetWidth(); j++)
			{
				dest[j * 4] = src[j * 4 + 2];
				dest[j * 4 + 1] = src[j * 4 + 1];
				dest[j * 4 + 2] = src[j * 4];
				dest[j * 4 + 3] = src[j * 4 + 3];
			}
			dest += rect.Pitch;
			src += image->GetWidth() * 4;
		}
		surface->UnlockRect();
	}

	Ptr<Texture> AssetManager::CreateTextureFromFile(const String& filePath)
	{
		auto exceptionManager = ExceptionManager::GetInstance();
		auto texture = Ptr<Texture>::New();
		auto device = Device::GetInstance()->GetD3DDevice();

		Ptr<Image> image = Image::Load(filePath);
		if (image == nullptr)
		{
			exceptionManager->PushException(Ptr<Exception>::New(
				((String)"无法加载纹理:" + filePath + "。原因是:\n" + exceptionManager->GetException()->GetInfo())));
			return nullptr;
		}

		texture->imageWidth = image->GetWidth();
		texture->imageHeight = image->GetHeight();

		int texWidth, texHeight;
		texWidth = texHeight = 1;
		while (texWidth < texture->imageWidth)
		{
			texWidth *= 2;
		}
		while (texHeight < texture->imageHeight)
		{
			texHeight *= 2;
		}
		texture->width = texWidth;
		texture->height = texHeight;

		if (FAILED(D3DXCreateTexture(device, texWidth, texHeight, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture->texture)))
		{
			exceptionManager->PushException(Ptr<Exception>::New(
				((String)"无法加载纹理:" + filePath + "。原因是:\nD3DXCreateTexture失败。")));
			return nullptr;
		}

		IDirect3DSurface9* surface;
		texture->texture->GetSurfaceLevel(0, &surface);
		CopyImageToSurface(image, surface);

		texture->xScale = (float)texture->imageWidth / texture->width;
		texture->yScale = (float)texture->imageHeight / texture->height;
		texture->name = filePath;

		TH_LOCK(this->mutex)
		{
			textureList.Add(texture);
		}

		return texture;
	}

	Ptr<CubeMap> AssetManager::CreateCubeMapFromFile(const String& front, const String& back,
		const String& left, const String& right, const String& top, const String& bottom)
	{
		auto exceptionManager = ExceptionManager::GetInstance();
		auto device = Device::GetInstance()->GetD3DDevice();
		auto cubeMap = Ptr<CubeMap>::New();

		Ptr<Image> frontImage = Image::Load(front);
		Ptr<Image> backImage = Image::Load(back);
		Ptr<Image> leftImage = Image::Load(left);
		Ptr<Image> rightImage = Image::Load(right);
		Ptr<Image> topImage = Image::Load(top);
		Ptr<Image> bottomImage = Image::Load(bottom);

		if (frontImage == nullptr || backImage == nullptr || leftImage == nullptr || rightImage == nullptr
			|| topImage == nullptr || bottomImage == nullptr)
		{
			exceptionManager->PushException(Ptr<Exception>::New(
				((String)"无法加载立方体纹理。原因是:\n" + exceptionManager->GetException()->GetInfo())));
			return nullptr;
		}

		int imgWidth = frontImage->GetWidth();
		int imgHeight = frontImage->GetHeight();
		if (imgWidth != imgHeight)
		{
			exceptionManager->PushException(Ptr<Exception>::New(
				((String)"无法加载立方体纹理。原因是:\n纹理长宽不相等。")));
			return nullptr;
		}

		if ((backImage->GetWidth() != imgWidth) || (backImage->GetHeight() != imgHeight)
			|| (leftImage->GetWidth() != imgWidth) || (leftImage->GetHeight() != imgHeight)
			|| (rightImage->GetWidth() != imgWidth) || (rightImage->GetHeight() != imgHeight)
			|| (topImage->GetWidth() != imgWidth) || (topImage->GetHeight() != imgHeight)
			|| (bottomImage->GetWidth() != imgWidth) || (bottomImage->GetHeight() != imgHeight))
		{
			exceptionManager->PushException(Ptr<Exception>::New(
				((String)"无法加载立方体纹理。原因是:\n各面纹理大小不一致")));
			return nullptr;
		}

		int texWidth, texHeight;
		texWidth = texHeight = 1;
		while (texWidth < imgWidth)
		{
			texWidth *= 2;
		}
		while (texHeight < imgHeight)
		{
			texHeight *= 2;
		}

		if (texWidth != imgWidth || texHeight != imgHeight)
		{
			float scaleX = (float)imgWidth / texWidth;
			float scaleY = (float)imgHeight / texHeight;
			//TODO : Scale the image.
		}

		if (FAILED(D3DXCreateCubeTexture(device, imgWidth, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED, &cubeMap->cubeTexture)))
		{
			exceptionManager->PushException(Ptr<Exception>::New(
				("无法加载立方体纹理。原因是:\nD3DXCreateCubeTexture失败。")));
			return nullptr;
		}

		//Copy image data to texture surfaces
		IDirect3DSurface9* surfaceFront = nullptr;
		IDirect3DSurface9* surfaceBack = nullptr;
		IDirect3DSurface9* surfaceLeft = nullptr;
		IDirect3DSurface9* surfaceRight = nullptr;
		IDirect3DSurface9* surfaceTop = nullptr;
		IDirect3DSurface9* surfaceBottom = nullptr;

		cubeMap->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &surfaceFront);
		cubeMap->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &surfaceBack);
		cubeMap->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &surfaceLeft);
		cubeMap->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &surfaceRight);
		cubeMap->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &surfaceTop);
		cubeMap->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &surfaceBottom);

		CopyImageToSurface(frontImage, surfaceFront);
		CopyImageToSurface(backImage, surfaceBack);
		CopyImageToSurface(leftImage, surfaceLeft);
		CopyImageToSurface(rightImage, surfaceRight);
		CopyImageToSurface(topImage, surfaceTop);
		CopyImageToSurface(bottomImage, surfaceBottom);

		TH_LOCK(this->mutex)
		{
			this->cubeMapList.Add(cubeMap);
		}

		return cubeMap;
	}

	Ptr<RenderTexture> AssetManager::CreateRenderTexture(int width, int height)
	{
		auto texture = Ptr<RenderTexture>::New();
		auto device = Device::GetInstance()->GetD3DDevice();

		texture->width = width;
		texture->height = height;
		D3DXCreateTexture(device, width, height, 0, D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture->texture);

		TH_LOCK(this->mutex)
		{
			textureList.Add(texture.Get());
		}

		return texture;
	}

	Ptr<FloatTexture> AssetManager::CreateFloatTexture(int width, int height)
	{
		auto texture = Ptr<FloatTexture>::New();
		auto device = Device::GetInstance()->GetD3DDevice();

		texture->width = width;
		texture->height = height;
		D3DXCreateTexture(device, width, height, 0, D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_RENDERTARGET, D3DFMT_G32R32F, D3DPOOL_DEFAULT, &texture->texture);

		TH_LOCK(this->mutex)
		{
			textureList.Add(texture.Get());
		}

		return texture;
	}

	void AssetManager::OnLostDevice()
	{
		auto iter = textureList.GetIterator();
		while (iter->HasNext())
		{
			iter->Next()->OnLostDevice();
		}

		auto iter2 = shaderList.GetIterator();
		while (iter2->HasNext())
		{
			iter2->Next()->OnLostDevice();
		}
	}

	void AssetManager::OnResetDevice()
	{
		auto iter = textureList.GetIterator();
		while (iter->HasNext())
		{
			iter->Next()->OnResetDevice();
		}

		auto iter2 = shaderList.GetIterator();
		while (iter2->HasNext())
		{
			iter2->Next()->OnResetDevice();
		}
	}

	void AssetManager::Update()
	{
		for (auto iter = textureList.GetIterator(); iter->HasNext();)
		{
			auto texture = iter->Next();
			if (texture->RefCount() == 2)  // one is refered here, anothre is refered inside the list.
				iter->Remove();
		}

		for (auto iter = cubeMapList.GetIterator(); iter->HasNext();)
		{
			auto cubeMap = iter->Next();
			if (cubeMap->RefCount() == 2) // one is refered here, anothre is refered inside the list.
				iter->Remove();
		}
	}
}