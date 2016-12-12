#ifndef THMESHRENDERER_H
#define THMESHRENDERER_H

#include "../Core/3D/THMesh.h"
#include "THRenderer.h"
#include "../Asset/THShader.h"

namespace THEngine
{
	class MeshRenderer : public Renderer
	{
	protected:
		Shader* meshShader = nullptr;

		IDirect3DVertexBuffer9* vertexBuffer = nullptr;
		IDirect3DIndexBuffer9* indexBuffer = nullptr;

	protected:
		void CalcWorldTransform(Mesh* mesh);

	private:
		void DrawD3DMesh(Mesh::D3DMesh* mesh);

		void SetupRenderState();

	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		virtual void Render(RenderObject* object) override;

		static MeshRenderer* Create();
	};
}

#endif