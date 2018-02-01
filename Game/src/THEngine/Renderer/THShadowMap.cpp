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
	}

	Ptr<ShadowMap> ShadowMap::Create(int width, int height)
	{
		ShadowMap* r = (ShadowMap*)malloc(sizeof(ShadowMap));
		new(r) ShadowMap();
		Ptr<ShadowMap> shadowMap = Ptr<ShadowMap>::Create_NoRetain(r);
		auto assetManager = AssetManager::GetInstance();

		shadowMap->shadowMap = assetManager->CreateFloatTexture(width, height);
		if (shadowMap->shadowMap == nullptr)
		{
			ExceptionManager::GetInstance()->PushException(Ptr<Exception>::New(("创建浮点纹理失败。")));
			return nullptr;
		}
		return shadowMap;
	}
}