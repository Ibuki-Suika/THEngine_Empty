#include "THCubeMap.h"
#include "THAssetManager.h"

namespace THEngine
{
	CubeMap::CubeMap()
	{
	}

	CubeMap::~CubeMap()
	{
		TH_SAFE_RELEASE(cubeTexture);
	}

	void CubeMap::OnLostDevice()
	{
	}

	void CubeMap::OnResetDevice()
	{
	}
}