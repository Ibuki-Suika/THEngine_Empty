#include "THCubeMap.h"
#include "THAssetManager.h"

namespace THEngine
{
	CubeMapImpl::CubeMapImpl()
	{

	}

	CubeMapImpl::~CubeMapImpl()
	{
		TH_SAFE_RELEASE(cubeTexture);
	}

	void CubeMapImpl::OnLostDevice()
	{

	}

	void CubeMapImpl::OnResetDevice()
	{

	}

	/////////////////////////////////////////////
	CubeMap::CubeMap()
	{

	}

	CubeMap::~CubeMap()
	{
		auto assetManager = AssetManager::GetInstance();
		assetManager->DestroyCubeMap(this);
	}
}