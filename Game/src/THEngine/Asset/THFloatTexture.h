#ifndef TH_FLOAT_TEXTURE_H
#define TH_FLOAT_TEXTURE_H

#include "THRenderTexture.h"

namespace THEngine
{
	class FloatTexture : public RenderTexture
	{
	public:
		FloatTexture() = default;
		virtual ~FloatTexture() = default;
	};
}

#endif
