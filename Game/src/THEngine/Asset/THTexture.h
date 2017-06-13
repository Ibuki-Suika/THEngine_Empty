#ifndef THTEXTURE_H
#define THTEXTURE_H

#include "THAssetImpl.h"

namespace THEngine
{
	class Image;

	class TextureImpl : public AssetImpl
	{
	private:
		IDirect3DTexture9* texture = nullptr;
		Image* texImage = nullptr;
		int width, height;
		int imageWidth, imageHeight;
		float xScale, yScale;

	protected:
		TextureImpl();

	public:
		virtual ~TextureImpl();

		virtual void OnLostDevice() override;
		virtual void OnResetDevice() override;

		bool SaveToFile(const String& path);

		void GenerateMipmap();

		friend class Texture;
		friend class Application;
		friend class AssetManager;
		friend class Shader;
		friend class SpriteRenderer;
	};

	class Texture : public Object
	{
	protected:
		TextureImpl* texImpl = nullptr;

		String name;

	public:
		Texture();
		virtual ~Texture();

		inline int GetWidth() const { return texImpl->width; }
		inline int GetHeight() const { return texImpl->height; }
		inline int GetImageWidth() const { return texImpl->imageWidth; }
		inline int GetImageHeight() const { return texImpl->imageHeight; }
		inline String GetName() const { return name; }

		inline bool SaveToFile(const String& path) const { return texImpl->SaveToFile(path); }

		inline void GenerateMipmap() { return texImpl->GenerateMipmap(); }

		friend class Application;
		friend class AssetManager;
		friend class Shader;
		friend class SpriteRenderer;
	};
}

#endif
