#ifndef THSPRITERENDERER_H
#define THSPRITERENDERER_H

#include "THRenderer.h"

namespace THEngine
{
	class Application;
	class Sprite;

	class SpriteRenderer : public Renderer
	{
	private:
		static const int MAX_SPRITE = 10000;
		static const DWORD SPRITE_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

		struct SpriteVertex
		{
			float x, y, z;
			D3DCOLOR color;
			float u, v;
			SpriteVertex() {}
			SpriteVertex(float x, float y, float z, float r, float g, float b, float a, float u, float v)
				:x(x), y(y), z(z), color(D3DCOLOR_ARGB((int)(255 * a), (int)(255 * r), (int)(255 * g), (int)(255 * b))), u(u), v(v)
			{
			}
		};

		/*struct SpriteBatch
		{
			SpriteVertex vertices[MAX_SPRITE][6];
			int spriteCount = 0;
			Texture* texture;
		};*/

		IDirect3DVertexBuffer9* vb = nullptr;
		//SpriteBatch spriteBatch;

	private:
		/*void PushBatch(Sprite* sprite);
		void CalcWorldTransform(Sprite* sprite, Matrix* transform);
		void SetupVertices(Sprite* sprite, const Matrix& transform);
		void FlushBatch();*/

	private:
		SpriteRenderer();
		virtual ~SpriteRenderer();

	public:
		static Ptr<SpriteRenderer> Create();

		virtual void Render(Ptr<GameObject> obj) override;

		friend class SpriteRenderQueue;
	};
}

#endif