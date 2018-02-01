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
		Ptr<SpriteRenderer> spriteRenderer;
		Ptr<Particle3DRenderer> particle3DRenderer = nullptr;
		Ptr<MeshRenderer> meshRenderer = nullptr;
		Ptr<SkyBoxRenderer> skyBoxRenderer = nullptr;
		Ptr<DirectionalLightShadowRenderer> dirShadowRenderer = nullptr;
		Ptr<DirectionalLightRenderer> dirLightRenderer = nullptr;

		Ptr<SpriteRenderQueue> spriteQueue = nullptr;
		Ptr<NormalRenderQueue> normalQueue = nullptr;

	private:
		RenderPipeline();

		void Render3D();
		void RenderWithLight(Ptr<Light> light);
		void RenderShadowMap(Ptr<Light> light);

	public:
		enum RenderQueueType
		{
			SPRITE,
			NORMAL
		};

	public:
		virtual ~RenderPipeline();

		static Ptr<RenderPipeline> Create();

		void Render();

		void SendToRenderQueue(RenderQueueType type, GameObject* obj);

		inline Ptr<SpriteRenderer> GetSpriteRenderer() const { return spriteRenderer; }
		inline Ptr<Particle3DRenderer> GetParticle3DRenderer() const { return particle3DRenderer; }
		inline Ptr<MeshRenderer> GetMeshRenderer() const { return meshRenderer; }
		inline Ptr<SkyBoxRenderer> GetSkyBoxRenderer() const { return skyBoxRenderer; }
	};
}

#endif