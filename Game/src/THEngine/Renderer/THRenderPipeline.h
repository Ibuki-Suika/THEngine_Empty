#ifndef TH_RENDER_PIPELINE_H
#define TH_RENDER_PIPELINE_H

#include <Common\THCommon.h>

namespace THEngine
{
	class SpriteRenderer;
	class Particle3DRenderer;
	class MeshRenderer;
	class SkyBoxRenderer;
	class ShadowRenderer;
	class RenderQueue;
	class DirectionalLightRenderer;
	class DirectionalLightShadowRenderer;
	class SpriteRenderQueue;
	class NormalRenderQueue;
	class Light;
	class GameObject;

	class RenderPipeline : public Object
	{
	private:
		SpriteRenderer* spriteRenderer = nullptr;
		Particle3DRenderer* particle3DRenderer = nullptr;
		MeshRenderer* meshRenderer = nullptr;
		SkyBoxRenderer* skyBoxRenderer = nullptr;
		DirectionalLightShadowRenderer* dirShadowRenderer = nullptr;
		DirectionalLightRenderer* dirLightRenderer = nullptr;

		SpriteRenderQueue* spriteQueue = nullptr;
		NormalRenderQueue* normalQueue = nullptr;

	private:
		RenderPipeline();

		void Render3D();
		void RenderWithLight(Light* light);
		void RenderShadowMap(Light* light);

	public:
		enum RenderQueueType
		{
			SPRITE,
			NORMAL
		};

	public:
		virtual ~RenderPipeline();

		static RenderPipeline* Create();

		void Render();

		void SendToRenderQueue(RenderQueueType type, GameObject* obj);

		inline SpriteRenderer* GetSpriteRenderer() const { return spriteRenderer; }
		inline Particle3DRenderer* GetParticle3DRenderer() const { return particle3DRenderer; }
		inline MeshRenderer* GetMeshRenderer() const { return meshRenderer; }
		inline SkyBoxRenderer* GetSkyBoxRenderer() const { return skyBoxRenderer; }
	};
}

#endif