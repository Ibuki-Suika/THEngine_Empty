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

		Light* currentLight = nullptr;

		bool isRenderingAmbient = false;

	protected:
		void SetupRenderState();

		void SetupShaderParams(Mesh* mesh);

		void Shade(Mesh* mesh);

		void ShadeWithAmbient(Mesh* mesh, const Vector4f& ambient);

		virtual void ShadeWithLight(Mesh* mesh);

		void DrawMesh(Mesh* mesh);

	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		virtual void Render(GameObject* object) override;

		void RenderObjects(RenderQueue* renderQueue);

		inline void SetRenderAmbient(bool renderAmbient) { this->isRenderingAmbient = renderAmbient; }
		inline void SetCurrentLight(Light* light) { this->currentLight = light; }

		static MeshRenderer* Create();
	};

	//////////////////////////////////////////////
	class DirectionalLightRenderer : public MeshRenderer
	{
	private:
		ShadowMap* shadowMapNear = nullptr;
		ShadowMap* shadowMapMid = nullptr;
		ShadowMap* shadowMapFar = nullptr;

	private:
		DirectionalLightRenderer();

	public:
		virtual ~DirectionalLightRenderer();

		static DirectionalLightRenderer* Create();

		virtual void ShadeWithLight(Mesh* mesh) override;

		inline void SetShadowMapNear(ShadowMap* shadowMap) { this->shadowMapNear = shadowMap; }
		inline void SetShadowMapMid(ShadowMap* shadowMap) { this->shadowMapMid = shadowMap; }
		inline void SetShadowMapFar(ShadowMap* shadowMap) { this->shadowMapFar = shadowMap; }
	};
}

#endif