#ifndef TH_SHADOW_MAP_H
#define TH_SHADOW_MAP_H

#include <Math\THMatrix.h>

namespace THEngine
{
	class FloatTexture;

	class ShadowMap : public Object
	{
	private:
		Ptr<FloatTexture> shadowMap;
		Matrix lightView;
		Matrix lightProjection;

	private:
		ShadowMap();

	public:
		virtual ~ShadowMap();

		static Ptr<ShadowMap> Create(int width, int height);

		inline Ptr<FloatTexture> GetShadowMap() const { return this->shadowMap; }
		inline const Matrix& GetLightView() const { return this->lightView; }
		inline const Matrix& GetLightProjection() const { return this->lightProjection; }

		friend class DirectionalLightShadowRenderer;
	};
}

#endif
