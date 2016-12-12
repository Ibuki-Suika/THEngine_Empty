#include "THParticle3DRenderer.h"
#include "../Core/THParticle3D.h"
#include "../Core/THGame.h"

using namespace THEngine;

Particle3DRenderer::Particle3DRenderer()
{

}

Particle3DRenderer::~Particle3DRenderer()
{

}

Particle3DRenderer* Particle3DRenderer::Create()
{
	Particle3DRenderer* renderer = new Particle3DRenderer();
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

		auto app = Application::GetInstance();

		renderer->device = app->GetDevice();
		renderer->device->CreateVertexBuffer(4 * sizeof(ParticleVertex), 0,
			D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_MANAGED, &renderer->vb, NULL);
		if (renderer->vb == nullptr)
		{
			ExceptionManager::GetInstance()->PushException(new Exception(("´´½¨¶¥µã»º´æÊ§°Ü¡£")));
			delete renderer;
			return nullptr;
		}
	}
	return renderer;
}

void Particle3DRenderer::Render(RenderObject* obj)
{
	Particle3D* particle = (Particle3D*)obj;

	auto app = Application::GetInstance();

	const int texWidth = particle->texture->GetWidth();
	const int texHeight = particle->texture->GetHeight();

	float width = particle->texRect.Width();
	float height = particle->texRect.Height();

	//Transform to texture coordinates
	double left = particle->texRect.left * 1.0 / texWidth;
	double top = particle->texRect.top * 1.0 / texHeight;
	double right = particle->texRect.right * 1.0 / texWidth;
	double bottom = particle->texRect.bottom * 1.0 / texHeight;

	if (left == 0 && top == 0 && right == 0 && bottom == 0)
	{
		width = particle->texture->GetImageWidth();
		height = particle->texture->GetImageHeight();
		left = top = 0;
		right = width / texWidth;
		bottom = height / texHeight;
	}
	left += 0.5 / texWidth;
	right += 0.5 / texWidth;
	top += 0.5 / texHeight;
	bottom += 0.5 / texHeight;

	float x = -width * particle->pivot.x;
	float y = -height * particle->pivot.y;
	float z = 0;

	ParticleVertex* vertices;
	vb->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);
	vertices[0] = ParticleVertex(x, y, z, left, bottom);
	vertices[1] = ParticleVertex(x + width, y, z, right, bottom);
	vertices[2] = ParticleVertex(x, y + height, z, left, top);
	vertices[3] = ParticleVertex(x + width, y + height, z, right, top);
	vb->Unlock();

	float argb[4];
	argb[0] = particle->alpha;
	argb[1] = particle->color.x;
	argb[2] = particle->color.y;
	argb[3] = particle->color.z;

	D3DXMATRIX transform;
	D3DXMatrixIdentity(&transform);

	if (particle->flipX)
	{
		transform._11 = -1;
	}
	if (particle->flipY)
	{
		transform._22 = -1;
	}

	D3DXMATRIX temp;

	D3DXMatrixScaling(&temp, particle->GetScale().x, particle->GetScale().y, particle->GetScale().z);
	transform *= temp;

	D3DXMatrixRotationQuaternion(&temp, &particle->rotation3D);
	transform *= temp;

	D3DXMatrixTranslation(&temp, floor(0.5f + particle->position.x), floor(0.5f + particle->position.y), particle->position.z);
	transform *= temp;

	app->SetWorldTransform(&transform);

	device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	device->SetStreamSource(0, vb, 0, sizeof(ParticleVertex));

	auto renderState = app->GetRenderState();

	spriteShader->Begin();

	spriteShader->SetFloatArray("argb", argb, 4);
	spriteShader->SetTexture("tex", particle->texture);
	spriteShader->SetInt("texWidth", texWidth);
	spriteShader->SetInt("texHeight", texHeight);
	spriteShader->SetMatrix("world", &renderState->world);
	spriteShader->SetMatrix("projection", &renderState->projection);
	spriteShader->SetMatrix("view", &renderState->view);

	spriteShader->BeginPass(0);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	spriteShader->EndPass();

	spriteShader->End();
}