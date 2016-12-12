#ifndef THPARTICLE3DRENDERER_H
#define THPARTICLE3DRENDERER_H

#include "THRenderer.h"

namespace THEngine
{
	class Particle3DRenderer : public Renderer
	{
	protected:
		IDirect3DVertexBuffer9* vb;
		Shader* spriteShader;

		struct ParticleVertex
		{
			float x, y, z;
			float u, v;
			ParticleVertex() {}
			ParticleVertex(float x, float y, float z, float u, float v) :x(x), y(y), z(z), u(u), v(v) {}
		};

	public:
		Particle3DRenderer();
		virtual ~Particle3DRenderer();

		virtual void Render(RenderObject* obj) override;

		static Particle3DRenderer* Create();
	};
}

#endif