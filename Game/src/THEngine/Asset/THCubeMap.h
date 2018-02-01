#ifndef TH_CUBE_MAP_H
#define TH_CUBE_MAP_H

#include "THBaseAsset.h"

namespace THEngine
{
	class CubeMap : public BaseAsset
	{
	private:
		IDirect3DCubeTexture9 * cubeTexture = nullptr;

	public:
		CubeMap();
		virtual ~CubeMap();

		virtual void OnLostDevice() override;
		virtual void OnResetDevice() override;

		friend class AssetManager;
		friend class Shader;
	};
}

#endif