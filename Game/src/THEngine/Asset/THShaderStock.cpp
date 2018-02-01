#include "THShaderStock.h"
#include "THAssetManager.h"
#include <Core\THGame.h>

namespace THEngine
{
	ShaderStock::~ShaderStock()
	{
	}

	bool ShaderStock::Init()
	{
		auto assetManager = AssetManager::GetInstance();
		auto exceptionManager = ExceptionManager::GetInstance();

		shadowShader = assetManager->CreateShaderFromFile("fx/shadow.fx");
		if (shadowShader != nullptr)
		{
			shadowShader->SetTechnique("Shadow");
		}
		else
		{
			exceptionManager->PushException(Ptr<Exception>::New("º”‘ÿshadow shader ß∞‹°£"));
			return false;
		}

		skyBoxShader = Game::GetInstance()->GetAssetManager()->CreateShaderFromFile("fx/skybox.fx");
		if (skyBoxShader != nullptr)
		{
			skyBoxShader->SetTechnique("SkyBox");
		}
		else
		{
			exceptionManager->PushException(Ptr<Exception>::New("º”‘ÿskybox shader ß∞‹°£"));
			return false;
		}

		meshShader = assetManager->CreateShaderFromFile("fx/mesh.fx");
		if (meshShader != nullptr)
		{
			meshShader->SetTechnique("Mesh");
		}
		else
		{
			exceptionManager->PushException(Ptr<Exception>::New("º”‘ÿmesh shader ß∞‹°£"));
			return false;
		}

		spriteShader = Game::GetInstance()->GetAssetManager()->CreateShaderFromFile("fx/sprite.fx");
		if (spriteShader != nullptr)
		{
			spriteShader->SetTechnique("Sprite");
		}
		else
		{
			exceptionManager->PushException(Ptr<Exception>::New("º”‘ÿsprite shader ß∞‹°£"));
			return false;
		}

		return true;
	}
}