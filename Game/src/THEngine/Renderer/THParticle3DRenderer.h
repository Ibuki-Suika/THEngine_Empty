#ifndef THPARTICLE3DRENDERER_H
#define THPARTICLE3DRENDERER_H

#include "THRenderer.h"

namespace THEngine
{
	class Particle3DRenderer : public Renderer
	{
	protected:
		IDirect3DVertexBuffer9* vb;

		struct ParticleVertex
		{
			float x, y, z;
			D3DCOLOR color;
			float u, v;
			ParticleVertex() {}
			ParticleVertex(float x, float y, float z, float r, float g, float b, float a, float u, float v)
				:x(x), y(y), z(z), color(D3DCOLOR_ARGB((int)(255 * a), (int)(255 * r), (int)(255 * g), (int)(255 * b))), u(u), v(v)
			{
			}
		};

		static const DWORD PARTICLE_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	private:
		Particle3DRenderer();

	public:
		virtual ~Particle3DRenderer();

		virtual void Render(Ptr<GameObject> obj) override;

		static Ptr<Particle3DRenderer> Create();
	};
}

#endif