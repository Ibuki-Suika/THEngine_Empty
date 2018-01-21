#include "THSprite.h"
#include "THGame.h"
#include <Math\THMath.h>
#include <Asset\THTexture.h>
#include <Renderer\THRenderPipeline.h>
#include <Renderer\THSpriteRenderer.h>

using namespace THEngine;

Sprite::Sprite()
{
	texRect = Rect(0, 0, 0, 0);
	alpha = 1.0f;
	color = Vector3f(1.0f, 1.0f, 1.0f);
	pivot = Vector2f(0.5f, 0.5f);
	rotation = 0;

	speed = 0;
	angle = 0;
	acSpeed = 0;
	acAngle = 0;

	flipX = false;
	flipY = false;
}

Sprite::~Sprite()
{
}

void Sprite::SetTexture(Ptr<Texture> texture)
{
	this->texture = texture;
}

void Sprite::SendToRenderQueue()
{
	Game::GetInstance()->GetRenderPipeline()->SendToRenderQueue(RenderPipeline::SPRITE, this);
}

void Sprite::Update()
{
	GameObject::Update();

	speed += acSpeed;
	angle += acAngle;
	vx = speed * cos(angle / 180.0f * Math::PI);
	vy = speed * sin(angle / 180.0f * Math::PI);
	vx += ax;
	vy += ay;
	position.x += vx;
	position.y += vy;
}

void Sprite::Draw()
{
	Game::GetInstance()->GetRenderPipeline()->GetSpriteRenderer()->Render(this);
}