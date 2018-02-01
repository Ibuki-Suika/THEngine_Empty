#ifndef THASSETMANAGER
#define THASSETMANAGER

#include <Common\THCommon.h>
#include "THShader.h"
#include "THTexture.h"
#include "THCubeMap.h"

namespace THEngine
{
	class RenderTexture;
	class FloatTexture;

	class AssetManager : public Object, public Singleton<AssetManager>
	{
		friend class Singleton<AssetManager>;
	private:
		ArrayList<Ptr<Shader>> shaderList;
		ArrayList<Ptr<Texture>> textureList;
		ArrayList<Ptr<CubeMap>> cubeMapList;

		std::mutex mutex;

	private:
		AssetManager() = default;
		void CopyImageToSurface(Ptr<Image> image, IDirect3DSurface9* surface);

	public:
		bool Init();

		Ptr<Shader> CreateShaderFromFile(const String& filePath);
		void DestroyShader(Ptr<Shader> shader);

		Ptr<Texture> CreateTextureFromFile(const String& filePath);
		Ptr<CubeMap> CreateCubeMapFromFile(const String& front, const String& back,
			const String& left, const String& right, const String& top, const String& bottom);
		Ptr<RenderTexture> CreateRenderTexture(int width, int height);
		Ptr<FloatTexture> CreateFloatTexture(int width, int height);

		void OnLostDevice();
		void OnResetDevice();
		void Update();
	};
}

#endif