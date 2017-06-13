#include "THAssetManager.h"
#include "THImage.h"
#include "THRenderTexture.h"
#include "THFloatTexture.h"
#include <Core\THGame.h>
#include <Platform\THApplication.h>

namespace THEngine
{
	AssetManager* AssetManager::instance = nullptr;

	AssetManager::AssetManager()
	{
		ASSERT(instance == nullptr);
		instance = this;
	}

	AssetManager* AssetManager::GetInstance()
	{
		return instance;
	}

	AssetManager* AssetManager::Create()
	{
		AssetManager* assetManager = new AssetManager();

		return assetManager;
	}

	Shader* AssetManager::CreateShaderFromFile(String filePath)
	{
		Shader* shader = new Shader();
		auto device = Application::GetInstance()->GetDevice();

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
			delete shader;
			return nullptr;
		}

		shader->path = filePath;

		shaderList.Add(shader);

		return shader;
	}

	void AssetManager::DestroyShader(Shader* shader)
	{
		shaderList.Remove(shader);
	}

	void AssetManager::CopyImageToSurface(Image* image, IDirect3DSurface9* surface)
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

	Texture* AssetManager::CreateTextureFromFile(String filePath)
	{
		auto exceptionManager = ExceptionManager::GetInstance();
		TextureImpl* texImpl = new TextureImpl();

		auto device = Application::GetInstance()->GetDevice();

		Image* image = Image::Load(filePath);
		if (image == nullptr)
		{
			exceptionManager->PushException(new Exception(
				((String)"无法加载纹理:" + filePath + "。原因是:\n" + exceptionManager->GetException()->GetInfo())));
			delete texImpl;
			return nullptr;
		}
		image->Retain();

		texImpl->imageWidth = image->GetWidth();
		texImpl->imageHeight = image->GetHeight();

		int texWidth, texHeight;
		texWidth = texHeight = 1;
		while (texWidth < texImpl->imageWidth)
		{
			texWidth *= 2;
		}
		while (texHeight < texImpl->imageHeight)
		{
			texHeight *= 2;
		}
		texImpl->width = texWidth;
		texImpl->height = texHeight;

		if (FAILED(D3DXCreateTexture(device, texWidth, texHeight, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texImpl->texture)))
		{
			exceptionManager->PushException(new Exception(
				((String)"无法加载纹理:" + filePath + "。原因是:\nD3DXCreateTexture失败。")));
			delete texImpl;
			return nullptr;
		}

		IDirect3DSurface9* surface;
		texImpl->texture->GetSurfaceLevel(0, &surface);
		CopyImageToSurface(image, surface);

		texImpl->xScale = (float)texImpl->imageWidth / texImpl->width;
		texImpl->yScale = (float)texImpl->imageHeight / texImpl->height;

		TH_SAFE_RELEASE(image);

		TH_LOCK(this->mutex)
		{
			textureList.Add(texImpl);
		}

		Texture* texture = new Texture();
		texture->name = filePath;
		TH_SET(texture->texImpl, texImpl);

		return texture;
	}

	CubeMap* AssetManager::CreateCubeMapFromFile(const String& front, const String& back,
		const String& left, const String& right, const String& top, const String& bottom)
	{
		CubeMapImpl* cubeMapImpl = new CubeMapImpl();
		auto exceptionManager = ExceptionManager::GetInstance();
		auto device = Application::GetInstance()->GetDevice();

		Image* frontImage = Image::Load(front);
		Image* backImage = Image::Load(back);
		Image* leftImage = Image::Load(left);
		Image* rightImage = Image::Load(right);
		Image* topImage = Image::Load(top);
		Image* bottomImage = Image::Load(bottom);

		if (frontImage == nullptr || backImage == nullptr || leftImage == nullptr || rightImage == nullptr
			|| topImage == nullptr || bottomImage == nullptr)
		{
			exceptionManager->PushException(new Exception(
				((String)"无法加载立方体纹理。原因是:\n" + exceptionManager->GetException()->GetInfo())));
			delete cubeMapImpl;
			return nullptr;
		}

		int imgWidth = frontImage->GetWidth();
		int imgHeight = frontImage->GetHeight();
		if (imgWidth != imgHeight)
		{
			exceptionManager->PushException(new Exception(
				((String)"无法加载立方体纹理。原因是:\n纹理长宽不相等。")));
			delete cubeMapImpl;
			return nullptr;
		}

		if ((backImage->GetWidth() != imgWidth) || (backImage->GetHeight() != imgHeight)
			|| (leftImage->GetWidth() != imgWidth) || (leftImage->GetHeight() != imgHeight)
			|| (rightImage->GetWidth() != imgWidth) || (rightImage->GetHeight() != imgHeight)
			|| (topImage->GetWidth() != imgWidth) || (topImage->GetHeight() != imgHeight)
			|| (bottomImage->GetWidth() != imgWidth) || (bottomImage->GetHeight() != imgHeight))
		{
			exceptionManager->PushException(new Exception(
				((String)"无法加载立方体纹理。原因是:\n各面纹理大小不一致")));
			delete cubeMapImpl;
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
			D3DPOOL_MANAGED, &cubeMapImpl->cubeTexture)))
		{
			exceptionManager->PushException(new Exception(
				("无法加载立方体纹理。原因是:\nD3DXCreateCubeTexture失败。")));
			delete cubeMapImpl;
			return nullptr;
		}

		//Copy image data to texture surfaces
		IDirect3DSurface9* surfaceFront = nullptr;
		IDirect3DSurface9* surfaceBack = nullptr;
		IDirect3DSurface9* surfaceLeft = nullptr;
		IDirect3DSurface9* surfaceRight = nullptr;
		IDirect3DSurface9* surfaceTop = nullptr;
		IDirect3DSurface9* surfaceBottom = nullptr;

		cubeMapImpl->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &surfaceFront);
		cubeMapImpl->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &surfaceBack);
		cubeMapImpl->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &surfaceLeft);
		cubeMapImpl->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &surfaceRight);
		cubeMapImpl->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &surfaceTop);
		cubeMapImpl->cubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &surfaceBottom);

		CopyImageToSurface(frontImage, surfaceFront);
		CopyImageToSurface(backImage, surfaceBack);
		CopyImageToSurface(leftImage, surfaceLeft);
		CopyImageToSurface(rightImage, surfaceRight);
		CopyImageToSurface(topImage, surfaceTop);
		CopyImageToSurface(bottomImage, surfaceBottom);

		delete frontImage;
		delete backImage;
		delete leftImage;
		delete rightImage;
		delete topImage;
		delete bottomImage;

		TH_LOCK(this->mutex)
		{
			this->cubeMapList.Add(cubeMapImpl);
		}

		CubeMap* cubeMap = new CubeMap();
		TH_SET(cubeMap->impl, cubeMapImpl);
		return cubeMap;
	}

	RenderTexture* AssetManager::CreateRenderTexture(int width, int height)
	{
		TextureImpl* texImpl = new TextureImpl();
		auto device = Application::GetInstance()->GetDevice();

		texImpl->width = width;
		texImpl->height = height;
		D3DXCreateTexture(device, width, height, 0, D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texImpl->texture);

		TH_LOCK(this->mutex)
		{
			textureList.Add(texImpl);
		}

		RenderTexture* texture = new RenderTexture();
		TH_SET(texture->texImpl, texImpl);

		return texture;
	}

	FloatTexture* AssetManager::CreateFloatTexture(int width, int height)
	{
		TextureImpl* texImpl = new TextureImpl();
		auto device = Application::GetInstance()->GetDevice();

		texImpl->width = width;
		texImpl->height = height;
		D3DXCreateTexture(device, width, height, 0, D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_RENDERTARGET, D3DFMT_G32R32F, D3DPOOL_DEFAULT, &texImpl->texture);

		TH_LOCK(this->mutex)
		{
			textureList.Add(texImpl);
		}

		FloatTexture* texture = new FloatTexture();
		TH_SET(texture->texImpl, texImpl);

		return texture;
	}

	void AssetManager::DestroyTexture(Texture* texture)
	{
		TH_LOCK(this->mutex)
		{
			textureList.Remove(texture->texImpl);
		}
		TH_SAFE_RELEASE(texture->texImpl);
	}

	void AssetManager::DestroyCubeMap(CubeMap* cubeMap)
	{
		TH_LOCK(this->mutex)
		{
			cubeMapList.Remove(cubeMap->impl);
		}
		TH_SAFE_RELEASE(cubeMap->impl);
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
}