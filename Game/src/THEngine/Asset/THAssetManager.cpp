#include "THAssetManager.h"
#include "THImage.h"

using namespace THEngine;

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

AssetManager* AssetManager::Create(Application* app)
{
	AssetManager* assetManager = new AssetManager();
	
	assetManager->device = app->device;

	assetManager->Retain();

	return assetManager;
}


Shader* AssetManager::CreateShaderFromFile(String filePath)
{
	Shader* shader = new Shader();
	
	ID3DXBuffer *error;
	if (FAILED(D3DXCreateEffectFromFile(device, filePath.GetBuffer(), NULL, NULL,
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

void AssetManager::DestroyShader(Shader* shader)
{
	shaderList.Remove(shader);
}

Texture* AssetManager::CreateTextureFromFile(String filePath)
{
	return CreateTextureFromFile(filePath, false);
}

Texture* AssetManager::CreateTextureFromFile(String filePath, bool useMipmap)
{
	auto exceptionManager = ExceptionManager::GetInstance();
	Texture* texture = new Texture();

	if (useMipmap)
	{
		if (FAILED(D3DXCreateTextureFromFile(device, filePath.GetBuffer(), &texture->texture)))
		{
			exceptionManager->PushException(new Exception(
				((String)"无法加载纹理:" + filePath + "。原因是:\n" + exceptionManager->GetException()->GetInfo())));
			delete texture;
			return nullptr;
		}
	}
	else
	{
		Image* image = Image::Load(filePath);
		if (image == nullptr)
		{
			exceptionManager->PushException(new Exception(
				((String)"无法加载纹理:" + filePath + "。原因是:\n" + exceptionManager->GetException()->GetInfo())));
			delete texture;
			return nullptr;
		}
		image->Retain();

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

		D3DXCreateTexture(device, texWidth, texHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture->texture);

		IDirect3DTexture9* tex = texture->texture;
		D3DLOCKED_RECT rect;
		RECT imageRect;
		imageRect.left = imageRect.top = 0;
		imageRect.right = texture->imageWidth;
		imageRect.bottom = texture->imageHeight;
		tex->LockRect(0, &rect, &imageRect, 0);
		unsigned char* dest = (unsigned char*)rect.pBits;
		unsigned char* src = (unsigned char*)image->GetData();
		for (int i = 0; i < texture->imageHeight; i++)
		{
			for (int j = 0; j < texture->imageWidth; j++)
			{
				dest[j * 4] = src[j * 4 + 2];
				dest[j * 4 + 1] = src[j * 4 + 1];
				dest[j * 4 + 2] = src[j * 4];
				dest[j * 4 + 3] = src[j * 4 + 3];
			}
			dest += rect.Pitch;
			src += texture->imageWidth * 4;
		}
		tex->UnlockRect(0);

		texture->xScale = (float)texture->imageWidth / texture->width;
		texture->yScale = (float)texture->imageHeight / texture->height;

		TH_SAFE_RELEASE(image);
	}

	textureList.Add(texture);

	return texture;
}

void AssetManager::DestroyTexture(Texture* texture)
{
	textureList.Remove(texture);
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
	device = Application::GetInstance()->GetDevice();

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