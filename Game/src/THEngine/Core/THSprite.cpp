#include "THSprite.h"
#include "THGame.h"
#include "../Math/MathUtil.h"

using namespace THEngine;

Sprite::Sprite()
{
	renderer = Game::GetInstance()->GetSpriteRenderer();
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

	texture = nullptr;
}

Sprite::~Sprite()
{

}

void Sprite::SendToRenderQueue()
{
	Game::GetInstance()->SendToRenderQueue(Game::SPRITE, this);
}

void Sprite::Update()
{
	RenderObject::Update();

	speed += acSpeed;
	angle += acAngle;
	vx = speed * cos(angle / 180.0f * PI);
	vy = speed * sin(angle / 180.0f * PI);
	vx += ax;
	vy += ay;
	position.x += vx;
	position.y += vy;
}

void Sprite::Draw()
{
	renderer->Render(this);
}