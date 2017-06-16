#include "THShaderStock.h"
#include "THAssetManager.h"
#include <Core\THGame.h>

namespace THEngine
{
	ShaderStock* ShaderStock::instance = nullptr;

	ShaderStock::ShaderStock()
	{
		ASSERT(instance == nullptr);
	}

	ShaderStock::~ShaderStock()
	{
		//TH_SAFE_RELEASE(this->shadowShader);
		//TH_SAFE_RELEASE(this->skyBoxShader);
	}
	
	ShaderStock* ShaderStock::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = Create();
		}
		return instance;
	}

	ShaderStock* ShaderStock::Create()
	{
		ASSERT(instance == nullptr);

		ShaderStock* stock = new ShaderStock();
		auto assetManager = AssetManager::GetInstance();
		auto exceptionManager = ExceptionManager::GetInstance();

		stock->shadowShader = assetManager->CreateShaderFromFile("fx/shadow.fx");
		if (stock->shadowShader)
		{
			stock->shadowShader->SetTechnique("Shadow");
		}
		else
		{
			exceptionManager->PushException(new Exception("º”‘ÿshadow shader ß∞‹°£"));
			delete stock;
			return nullptr;
		}

		stock->skyBoxShader = Game::GetInstance()->GetAssetManager()->CreateShaderFromFile("fx/skybox.fx");
		if (stock->skyBoxShader)
		{
			stock->skyBoxShader->SetTechnique("SkyBox");
		}
		else
		{
			exceptionManager->PushException(new Exception("º”‘ÿskybox shader ß∞‹°£"));
			delete stock;
			return nullptr;
		}

		stock->meshShader = assetManager->CreateShaderFromFile("fx/mesh.fx");
		if (stock->meshShader)
		{
			stock->meshShader->SetTechnique("Mesh");
		}
		else
		{
			exceptionManager->PushException(new Exception("º”‘ÿmesh shader ß∞‹°£"));
			delete stock;
			return nullptr;
		}

		stock->spriteShader = Game::GetInstance()->GetAssetManager()->CreateShaderFromFile("fx/sprite.fx");
		if (stock->spriteShader)
		{
			stock->spriteShader->SetTechnique("Sprite");
		}
		else
		{
			exceptionManager->PushException(new Exception("º”‘ÿsprite shader ß∞‹°£"));
			delete stock;
			return nullptr;
		}

		instance = stock;

		return stock;
	}
}