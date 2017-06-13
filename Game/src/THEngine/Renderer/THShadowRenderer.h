#ifndef TH_SHADOW_RENDERER_H
#define TH_SHADOW_RENDERER_H

#include "THRenderer.h"
#include "THShadowMap.h"
#include <Math\THMatrix.h>
#include <Core\THRenderState.h>

namespace THEngine
{
	class RenderQueue;
	class ShadowMap;
	class RenderTexture;
	class Surface;

	class ShadowRenderer : public Renderer
	{
	protected:
		Surface* depthBuffer = nullptr;
		bool depthTestEnabledPrev = true;

		Matrix projPrev;
		Matrix viewPrev;
		RenderTexture* renderTargetPrev = nullptr;
		Viewport viewportPrev;
		Surface* depthBufferPrev = nullptr;
		BlendMode blendModePrev;

		static const int SHADOW_MAP_SIZE = 256;

	protected:
		ShadowRenderer();

	public:
		virtual ~ShadowRenderer();

		virtual void Render(GameObject* obj) override;

		virtual void RenderShadow(RenderQueue* renderQueue) = 0;

		void Begin();
		void End();
	};

	class DirectionalLightShadowRenderer : public ShadowRenderer
	{
	private:
		struct BoundingBox
		{
			float xmin;
			float xmax;
			float ymin;
			float ymax;
			float zmin;
			float zmax;
		};

		ArrayList<ShadowMap*> cascadedShadowMaps;
		DirectionalLight* light = nullptr;
		Matrix lightView;

		static const int MAX_CASCADED_LEVEL = 3;

	private:
		void SetupLightProjection(int cascadedLevel);
		void SetupLightView();
		void FindAABB(const Vector4f* vertices, int size, BoundingBox& bbox);
		void FindAABB(const Vector3f* vertices, int size, BoundingBox& bbox);
		void FindBoundingSphere(const Vector3f* vertices, int size, Vector3f& center, float& radius);
		void FindAABBForSphere(const Vector3f& center, float radius, BoundingBox& bbox);
		void CreateVerticesFromAABB(const BoundingBox& bbox, Vector4f* output);
		void ComputeFrustum(int cascadedLevel, Vector3f* output);
		void ExpandAABB(BoundingBox& bbox, float rate);

	private:
		DirectionalLightShadowRenderer();

	public:
		virtual ~DirectionalLightShadowRenderer();

		static DirectionalLightShadowRenderer* Create();

		virtual void RenderShadow(RenderQueue* renderQueue) override;

		inline ArrayList<ShadowMap*>* GetCascadedShadowMaps() { return &this->cascadedShadowMaps; }

		inline void SetLight(DirectionalLight* light) { TH_SET(this->light, light); }
	};
}

#endif 
