#ifndef TH_SKY_BOX_RENDERER_H
#define TH_SKY_BOX_RENDERER_H

#include "THRenderer.h"

namespace THEngine
{
	class Camera3D;
	class Layer;

	class SkyBoxRenderer : public Object
	{
	private:
		struct SkyBoxVertex
		{
			float x, y, z;
			SkyBoxVertex() {}
			SkyBoxVertex(float x, float y, float z) :x(x), y(y), z(z) {}
		};

		IDirect3DVertexBuffer9* vb = nullptr;

		const static DWORD SKYBOX_FVF = D3DFVF_XYZ;

	private:
		SkyBoxRenderer();
		virtual ~SkyBoxRenderer();

	public:
		void RenderSkyBox(Ptr<Camera3D> camera, Ptr<Layer> layer);

		static Ptr<SkyBoxRenderer> Create();
	};
}

#endif