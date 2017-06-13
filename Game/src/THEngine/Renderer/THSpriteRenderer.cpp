#include "THSpriteRenderer.h"
#include <Core\THGame.h>
#include <Core\THSprite.h>
#include <Asset\THShaderStock.h>
#include <Asset\THTexture.h>
#include <Asset\THShader.h>
#include <Platform\THApplication.h>

using namespace THEngine;

SpriteRenderer::SpriteRenderer()
{
}

SpriteRenderer::~SpriteRenderer()
{
	TH_SAFE_RELEASE(vb);
}

SpriteRenderer* SpriteRenderer::Create()
{
	auto app = Application::GetInstance();
	SpriteRenderer* renderer = new SpriteRenderer();
	if (renderer)
	{
		renderer->device = app->GetDevice();
		renderer->device->CreateVertexBuffer(4 * sizeof(SpriteVertex), D3DUSAGE_DYNAMIC,
			SPRITE_FVF, D3DPOOL_DEFAULT, &renderer->vb, NULL);
		if (renderer->vb == nullptr)
		{
			ExceptionManager::GetInstance()->PushException(new Exception(("¥¥Ω®∂•µ„ª∫¥Ê ß∞‹°£")));
			delete renderer;
			return nullptr;
		}
	}
	return renderer;
}

void SpriteRenderer::Render(GameObject* obj)
{
	auto sprite = dynamic_cast<Sprite*>(obj);
	ASSERT(sprite != nullptr);

	auto game = Game::GetInstance();
	auto app = Application::GetInstance();
	auto spriteShader = ShaderStock::GetInstance()->GetSpriteShader();

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

	float x = -width * sprite->pivot.x;
	float y = -height * sprite->pivot.y;
	float z = sprite->position.z;

	//left += 0.5 / texWidth;
	//right += 0.5 / texWidth;
	//top += 0.5 / texHeight;
	//bottom += 0.5 / texHeight;

	SpriteVertex* vertices;
	auto& color = sprite->GetColor();
	vb->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);
	vertices[0] = SpriteVertex(x, y, z, color.x, color.y, color.z, sprite->GetAlpha(), left, bottom);
	vertices[1] = SpriteVertex(x + width, y, z, color.x, color.y, color.z, sprite->GetAlpha(), right, bottom);
	vertices[2] = SpriteVertex(x, y + height, z, color.x, color.y, color.z, sprite->GetAlpha(), left, top);
	vertices[3] = SpriteVertex(x + width, y + height, z, color.x, color.y, color.z, sprite->GetAlpha(), right, top);

	vb->Unlock();

	Matrix transform;
	Matrix::Identity(&transform);

	if (sprite->flipX)
	{
		transform.Set(0, 0, -1);
	}
	if (sprite->flipY)
	{
		transform.Set(1, 1, -1);
	}

	Matrix temp;
	Matrix::Scale(&temp, sprite->GetScaleX(), sprite->GetScaleY(), 1.0f);
	transform *= temp;

	Matrix::RotateZ(&temp, sprite->rotation);
	transform *= temp;

	if (sprite->pixelAlign)
	{
		Matrix::Translate(&temp, floor(sprite->positionForRender.x + 0.5f), floor(sprite->positionForRender.y + 0.5f), 0.0f);
	}
	else
	{
		Matrix::Translate(&temp, sprite->positionForRender.x, sprite->positionForRender.y, 0.0f);
	}
	transform *= temp;

	app->SetWorldMatrix(transform);

	device->SetFVF(SPRITE_FVF);
	device->SetStreamSource(0, vb, 0, sizeof(SpriteVertex));

	auto renderState = app->GetRenderState();

	spriteShader->Use();

	SetupRenderState(obj);

	spriteShader->SetTexture("tex", sprite->texture);
	spriteShader->SetInt("screenWidth", game->GetWidth());
	spriteShader->SetInt("screenHeight", game->GetHeight());
	spriteShader->SetInt("viewportWidth", renderState->GetViewport().width);
	spriteShader->SetInt("viewportHeight", renderState->GetViewport().height);
	spriteShader->SetMatrix("world", renderState->GetWorldMatrix());
	spriteShader->SetMatrix("projection", renderState->GetProjectionMatrix());
	spriteShader->SetMatrix("view", renderState->GetViewMatrix());
	spriteShader->SetBoolean("pixelAlign", sprite->pixelAlign);

	spriteShader->CommitChanges();
	spriteShader->UsePass(0);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
/*Sprite* sprite = (Sprite*)obj;
if (this->spriteBatch.texture && this->spriteBatch.spriteCount < MAX_SPRITE
	&& this->spriteBatch.texture == sprite->GetTexture())
{
	PushBatch(sprite);
}
else
{
	FlushBatch();
	PushBatch(sprite);
}
}

void SpriteRenderer::PushBatch(Sprite* sprite)
{
	if (this->spriteBatch.spriteCount < MAX_SPRITE)
	{
		if (this->spriteBatch.texture == nullptr)
		{
			this->spriteBatch.texture = sprite->texture;
		}
		//Calculate transform matrix
		Matrix transform;
		CalcWorldTransform(sprite, &transform);

		SetupVertices(sprite, transform);

		this->spriteBatch.spriteCount++;
	}
}

void SpriteRenderer::CalcWorldTransform(Sprite* sprite, Matrix* transform)
{
	Matrix::Identity(transform);

	if (sprite->flipX)
	{
		transform->Set(0, 0, -1);
	}
	if (sprite->flipY)
	{
		transform->Set(1, 1, -1);
	}

	Matrix temp;
	Matrix::Scale(&temp, sprite->GetScaleX(), sprite->GetScaleY(), 1.0f);
	*transform *= temp;

	Matrix::RotateZ(&temp, sprite->rotation);
	*transform *= temp;

	if (sprite->pixelAlign)
	{
		Matrix::Translate(&temp, floor(sprite->positionForRender.x + 0.5f), floor(sprite->positionForRender.y + 0.5f), 0.0f);
	}
	else
	{
		Matrix::Translate(&temp, sprite->positionForRender.x, sprite->positionForRender.y, 0.0f);
	}
	*transform *= temp;
}

void SpriteRenderer::SetupVertices(Sprite* sprite, const Matrix& transform)
{
	auto game = Game::GetInstance();
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

	float x = -width * sprite->pivot.x;
	float y = -height * sprite->pivot.y;
	float z = sprite->position.z;

	left += 0.5 / texWidth;
	right += 0.5 / texWidth;
	top += 0.5 / texHeight;
	bottom += 0.5 / texHeight;

	SpriteVertex* vertices = this->spriteBatch.vertices[spriteBatch.spriteCount];
	auto& color = sprite->GetColor();
	vertices[0] = SpriteVertex(x, y, z, color.x, color.y, color.z, sprite->GetAlpha(), left, bottom);
	vertices[1] = SpriteVertex(x + width, y, z, color.x, color.y, color.z, sprite->GetAlpha(), right, bottom);
	vertices[2] = SpriteVertex(x, y + height, z, color.x, color.y, color.z, sprite->GetAlpha(), left, top);
	vertices[3] = SpriteVertex(x, y + height, z, color.x, color.y, color.z, sprite->GetAlpha(), left, top);
	vertices[4] = SpriteVertex(x + width, y + height, z, color.x, color.y, color.z, sprite->GetAlpha(), right, top);
	vertices[5] = SpriteVertex(x + width, y, z, color.x, color.y, color.z, sprite->GetAlpha(), right, bottom);
	for (int i = 0; i < 6; i++)
	{
		Vector4f pos;
		pos.x = vertices[i].x;
		pos.y = vertices[i].y;
		pos.z = vertices[i].z;
		pos.w = 1.0f;
		pos *= transform;
		vertices[i].x = pos.x;
		vertices[i].y = pos.y;
		vertices[i].z = pos.z;
	}
}

void SpriteRenderer::FlushBatch()
{
	if (this->spriteBatch.spriteCount > 0)
	{
		auto game = Game::GetInstance();
		auto app = Application::GetInstance();

		Matrix transform;
		Matrix::Identity(&transform);
		app->SetWorldMatrix(transform);

		SpriteVertex* vertices;
		vb->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);
		memcpy(vertices, this->spriteBatch.vertices, this->spriteBatch.spriteCount * sizeof(SpriteVertex) * 6);
		vb->Unlock();

		device->SetFVF(SPRITE_FVF);
		device->SetStreamSource(0, vb, 0, sizeof(SpriteVertex));

		auto renderState = app->GetRenderState();

		spriteShader->Use();

		spriteShader->SetTexture("tex", spriteBatch.texture);
		spriteShader->SetInt("screenWidth", game->GetWidth());
		spriteShader->SetInt("screenHeight", game->GetHeight());
		spriteShader->SetInt("viewportWidth", renderState->GetViewport().width);
		spriteShader->SetInt("viewportHeight", renderState->GetViewport().height);
		spriteShader->SetMatrix("world", renderState->GetWorldMatrix());
		spriteShader->SetMatrix("projection", renderState->GetProjectionMatrix());
		spriteShader->SetMatrix("view", renderState->GetViewMatrix());

		spriteShader->UsePass(0);
		spriteShader->CommitChanges();
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2 * this->spriteBatch.spriteCount);
	}

	this->spriteBatch.texture = nullptr;
	this->spriteBatch.spriteCount = 0;
}*/