#include "THRenderer.h"
#include "../Core/THGame.h"

using namespace THEngine;

SpriteRenderer::SpriteRenderer()
{

}

SpriteRenderer::~SpriteRenderer()
{
	Game::GetInstance()->GetAssetManager()->DestroyShader(spriteShader);
	TH_SAFE_RELEASE(vb);
}

SpriteRenderer* SpriteRenderer::Create(Application* app)
{
	SpriteRenderer* renderer = new SpriteRenderer();
	if (renderer)
	{
		renderer->spriteShader = Game::GetInstance()->GetAssetManager()->CreateShaderFromFile("fx/sprite.fx");
		if (renderer->spriteShader)
		{
			renderer->spriteShader->SetTechnique("Sprite");
		}
		else
		{
			delete renderer;
			return nullptr;
		}

		renderer->device = app->device;
		renderer->device->CreateVertexBuffer(4 * sizeof(SpriteVertex), 0,
			D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_MANAGED, &renderer->vb, NULL);
		if (renderer->vb == nullptr)
		{
			ExceptionManager::GetInstance()->PushException(new Exception(("´´½¨¶¥µã»º´æÊ§°Ü¡£")));
			delete renderer;
			return nullptr;
		}
		renderer->Retain();
	}
	return renderer;
}

void SpriteRenderer::Render(GameObject* obj)
{
	auto sprite = dynamic_cast<Sprite*>(obj);
	ASSERT(sprite != nullptr);

	auto app = Application::GetInstance();

	const int texWidth = sprite->texture->GetWidth();
	const int texHeight = sprite->texture->GetHeight();

	float width = sprite->texRect.Width();
	float height = sprite->texRect.Height();

	//Transform to texture coordinates
	double left = sprite->texRect.left * 1.0 / texWidth;
	double top = sprite->texRect.top * 1.0 / texHeight;
	double right = sprite->texRect.right * 1.0 / texWidth;
	double bottom = sprite->texRect.bottom * 1.0 / texHeight;

	if (left == 0 && top == 0 && right == 0 && bottom == 0)
	{
		width = sprite->texture->GetImageWidth();
		height = sprite->texture->GetImageHeight();
		left = top = 0;
		right = width / texWidth;
		bottom = height / texHeight;
	}
	left += 0.5 / texWidth;
	right += 0.5 / texWidth;
	top += 0.5 / texHeight;
	bottom += 0.5 / texHeight;
	
	float x = -width * sprite->pivot.x;
	float y = -height * sprite->pivot.y;
	float z = sprite->position.z;

	SpriteVertex* vertices;
	vb->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);
	vertices[0] = SpriteVertex(x, y, z, left, bottom);
	vertices[1] = SpriteVertex(x + width, y, z, right, bottom);
	vertices[2] = SpriteVertex(x, y + height, z, left, top);
	vertices[3] = SpriteVertex(x + width, y + height, z, right, top);
	vb->Unlock();

	float argb[4];
	argb[0] = sprite->alpha;
	argb[1] = sprite->color.x;
	argb[2] = sprite->color.y;
	argb[3] = sprite->color.z;

	spriteShader->Begin();

	D3DXMATRIX transform;
	D3DXMatrixIdentity(&transform);

	if (sprite->flipX)
	{
		transform._11 = -1;
	}
	if (sprite->flipY)
	{
		transform._22 = -1;
	}

	D3DXMATRIX temp;

	D3DXMatrixScaling(&temp, sprite->GetScaleX(), sprite->GetScaleY(), 1.0f);
	transform *= temp;

	D3DXMatrixRotationZ(&temp, ToRad(sprite->rotation));
	transform *= temp;

	D3DXMatrixTranslation(&temp, floor(0.5f + sprite->position.x), floor(0.5f + sprite->position.y), 0.0f);
	transform *= temp;

	app->SetWorldTransform(&transform);

	device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	device->SetStreamSource(0, vb, 0, sizeof(SpriteVertex));

	spriteShader->SetFloatArray("argb", argb, 4);
	spriteShader->SetTexture("tex", sprite->texture);
	spriteShader->SetInt("texWidth", texWidth);
	spriteShader->SetInt("texHeight", texHeight);
	spriteShader->SetMatrix("world", &app->world);
	spriteShader->SetMatrix("projection", &app->projection);
	spriteShader->SetMatrix("view", &app->view);

	spriteShader->BeginPass(0);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	spriteShader->EndPass();

	spriteShader->End();
}