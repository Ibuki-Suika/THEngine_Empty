#ifndef TH_RENDER_TEXTURE_H
#define TH_RENDER_TEXTURE_H

#include "THTexture.h"

namespace THEngine
{
	class RenderTexture : public Texture
	{
	public:
		RenderTexture();
		virtual ~RenderTexture();
	};
}

#endif