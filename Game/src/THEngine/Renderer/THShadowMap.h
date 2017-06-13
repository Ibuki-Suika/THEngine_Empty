#ifndef TH_SHADOW_MAP_H
#define TH_SHADOW_MAP_H

#include <Math\THMatrix.h>

namespace THEngine
{
	class FloatTexture;

	class ShadowMap : public Object
	{
	private:
		FloatTexture* shadowMap = nullptr;
		Matrix lightView;
		Matrix lightProjection;

	public:
		ShadowMap();
		virtual ~ShadowMap();

		static ShadowMap* Create(int width, int height);

		inline FloatTexture* GetShadowMap() const { return this->shadowMap; }
		inline const Matrix& GetLightView() const { return this->lightView; }
		inline const Matrix& GetLightProjection() const { return this->lightProjection; }

		friend class DirectionalLightShadowRenderer;
	};
}

#endif
