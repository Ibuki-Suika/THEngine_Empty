#include "THShadowMap.h"
#include <Asset\THFloatTexture.h>
#include <Asset\THAssetManager.h>

namespace THEngine
{
	ShadowMap::ShadowMap()
	{

	}

	ShadowMap::~ShadowMap()
	{
		TH_SAFE_RELEASE(this->shadowMap);
	}

	ShadowMap* ShadowMap::Create(int width, int height)
	{
		ShadowMap* shadowMap = new ShadowMap();
		auto assetManager = AssetManager::GetInstance();

		shadowMap->shadowMap = assetManager->CreateFloatTexture(width, height);
		if (shadowMap->shadowMap == nullptr)
		{
			ExceptionManager::GetInstance()->PushException(new Exception(("创建浮点纹理失败。")));
			delete shadowMap;
			return nullptr;
		}
		shadowMap->shadowMap->Retain();

		return shadowMap;
	}
}