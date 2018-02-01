#ifndef TH_MESH_RENDERER_H
#define TH_MESH_RENDERER_H

#include "THRenderer.h"

namespace THEngine
{
	class FloatTexture;
	class ShadowMap;
	class Mesh;
	class Light;
	struct Vector4f;
	class RenderQueue;

	class MeshRenderer : public Renderer
	{
	protected:
		IDirect3DVertexBuffer9* vertexBuffer = nullptr;
		IDirect3DIndexBuffer9* indexBuffer = nullptr;

		Ptr<Light> currentLight;

		bool isRenderingAmbient = false;

	protected:
		void SetupRenderState();

		void SetupShaderParams(Mesh* mesh);

		void Shade(Mesh* mesh);

		void ShadeWithAmbient(Mesh* mesh, const Vector4f& ambient);

		virtual void ShadeWithLight(Mesh* mesh);

		void DrawMesh(Mesh* mesh);

	protected:
		MeshRenderer();

	public:
		virtual ~MeshRenderer();

		virtual void Render(Ptr<GameObject> object) override;

		void RenderObjects(Ptr<RenderQueue> renderQueue);

		inline void SetRenderAmbient(bool renderAmbient) { this->isRenderingAmbient = renderAmbient; }
		inline void SetCurrentLight(Ptr<Light> light) { this->currentLight = light; }

		static Ptr<MeshRenderer> Create();
	};

	//////////////////////////////////////////////
	class DirectionalLightRenderer : public MeshRenderer
	{
	private:
		Ptr<ShadowMap> shadowMapNear;
		Ptr<ShadowMap> shadowMapMid;
		Ptr<ShadowMap> shadowMapFar;

	private:
		DirectionalLightRenderer();

	public:
		virtual ~DirectionalLightRenderer();

		static Ptr<DirectionalLightRenderer> Create();

		virtual void ShadeWithLight(Mesh* mesh) override;

		inline void SetShadowMapNear(Ptr<ShadowMap> shadowMap) { this->shadowMapNear = shadowMap; }
		inline void SetShadowMapMid(Ptr<ShadowMap> shadowMap) { this->shadowMapMid = shadowMap; }
		inline void SetShadowMapFar(Ptr<ShadowMap> shadowMap) { this->shadowMapFar = shadowMap; }
	};
}

#endif