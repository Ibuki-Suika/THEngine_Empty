#ifndef TH_CUBE_MAP_H
#define TH_CUBE_MAP_H

#include "THAssetImpl.h"

namespace THEngine
{
	class CubeMapImpl : public AssetImpl
	{
	private:
		IDirect3DCubeTexture9* cubeTexture = nullptr;

	protected:
		CubeMapImpl();

	public:
		virtual ~CubeMapImpl();

		virtual void OnLostDevice() override;
		virtual void OnResetDevice() override;

		friend class AssetManager;
		friend class Shader;
	};

	class CubeMap : public Object
	{
	private:
		CubeMapImpl* impl = nullptr;  //don't use smart pointer here

	public:
		CubeMap();
		virtual ~CubeMap();

		friend class AssetManager;
		friend class Shader;
	};
}

#endif