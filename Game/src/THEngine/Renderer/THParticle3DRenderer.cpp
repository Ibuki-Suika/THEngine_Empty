#include "THParticle3DRenderer.h"
#include <Core\THParticle3D.h>
#include <Core\THGame.h>
#include <Asset\THShaderStock.h>
#include <Asset\THTexture.h>
#include <Asset\THShader.h>
#include <Platform\THDevice.h>

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
		auto device = Device::GetInstance();

		auto d3dDevice = device->GetD3DDevice();
		d3dDevice->CreateVertexBuffer(4 * sizeof(ParticleVertex), D3DUSAGE_DYNAMIC,
			PARTICLE_FVF, D3DPOOL_DEFAULT, &renderer->vb, NULL);
		if (renderer->vb == nullptr)
		{
			ExceptionManager::GetInstance()->PushException(new Exception(("´´½¨¶¥µã»º´æÊ§°Ü¡£")));
			delete renderer;
			return nullptr;
		}
	}
	return renderer;
}

void Particle3DRenderer::Render(GameObject* obj)
{
	Particle3D* particle = (Particle3D*)obj;

	auto spriteShader = ShaderStock::GetInstance()->GetSpriteShader();
	auto device = Device::GetInstance();

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
	auto& color = particle->GetColor();
	vb->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);
	vertices[0] = ParticleVertex(x, y, z, color.x, color.y, color.z, particle->alpha, left, bottom);
	vertices[1] = ParticleVertex(x + width, y, z, color.x, color.y, color.z, particle->alpha, right, bottom);
	vertices[2] = ParticleVertex(x, y + height, z, color.x, color.y, color.z, particle->alpha, left, top);
	vertices[3] = ParticleVertex(x + width, y + height, z, color.x, color.y, color.z, particle->alpha, right, top);
	vb->Unlock();

	float argb[4];
	argb[0] = particle->alpha;
	argb[1] = particle->color.x;
	argb[2] = particle->color.y;
	argb[3] = particle->color.z;

	Matrix transform;
	Matrix::Identity(&transform);

	if (particle->flipX)
	{
		transform.Set(0, 0, -1);
	}
	if (particle->flipY)
	{
		transform.Set(1, 1, -1);
	}

	Matrix temp;

	Matrix::Scale(&temp, particle->GetScale().x, particle->GetScale().y, particle->GetScale().z);
	transform *= temp;

	Matrix::RotateQuarternion(&temp, particle->rotation3D);
	transform *= temp;

	Matrix::Translate(&temp, floor(0.5f + particle->positionForRender.x), floor(0.5f + particle->positionForRender.y),
		particle->positionForRender.z);
	transform *= temp;

	device->SetWorldMatrix(transform);

	device->GetD3DDevice()->SetFVF(PARTICLE_FVF);
	device->GetD3DDevice()->SetStreamSource(0, vb, 0, sizeof(ParticleVertex));

	auto renderState = device->GetRenderState();

	spriteShader->Use();

	spriteShader->SetFloatArray("argb", argb, 4);
	spriteShader->SetTexture("tex", particle->texture);
	spriteShader->SetInt("texWidth", texWidth);
	spriteShader->SetInt("texHeight", texHeight);
	spriteShader->SetMatrix("world", renderState->GetWorldMatrix());
	spriteShader->SetMatrix("projection", renderState->GetProjectionMatrix());
	spriteShader->SetMatrix("view", renderState->GetViewMatrix());

	spriteShader->CommitChanges();
	spriteShader->UsePass(0);
	device->GetD3DDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}