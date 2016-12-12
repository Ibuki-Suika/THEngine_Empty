#ifndef THSPRITERENDERER_H
#define THSPRITERENDERER_H

#include "THRenderer.h"
#include "../Core/THSprite.h"

namespace THEngine
{
	class Application;

	class SpriteRenderer : public Renderer
	{
	private:
		Shader* spriteShader;
		IDirect3DVertexBuffer9* vb;

		struct SpriteVertex
		{
			float x, y, z;
			float u, v;
			SpriteVertex() {}
			SpriteVertex(float x, float y, float z, float u, float v) :x(x), y(y), z(z), u(u), v(v) {}
		};

	public:
		SpriteRenderer();
		virtual ~SpriteRenderer();

		static SpriteRenderer* Create(Application* app);

		virtual void Render(RenderObject* obj) override;
		void Render(Sprite* sprite);

		friend class SpriteRenderQueue;
	};

}

#endif