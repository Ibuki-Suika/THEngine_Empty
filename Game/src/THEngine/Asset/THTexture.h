#ifndef THTEXTURE_H
#define THTEXTURE_H

#include "THBaseAsset.h"

namespace THEngine
{
	class Image;

	class Texture : public BaseAsset
	{
	protected:
		IDirect3DTexture9 * texture = nullptr;
		Ptr<Image> texImage;    //used for device lost
		int width, height;
		int imageWidth, imageHeight;
		float xScale, yScale;
		String name;

	public:
		Texture();
		virtual ~Texture();

		inline int GetWidth() const { return this->width; }
		inline int GetHeight() const { return this->height; }
		inline int GetImageWidth() const { return this->imageWidth; }
		inline int GetImageHeight() const { return this->imageHeight; }
		inline const String& GetName() const { return this->name; }

		bool SaveToFile(const String& path);

		void GenerateMipmap();
		virtual void OnLostDevice() override;
		virtual void OnResetDevice() override;

		friend class Device;
		friend class AssetManager;
		friend class Shader;
		friend class SpriteRenderer;
	};
}

#endif
