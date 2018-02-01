#include "THConcreteTween.h"
#include "THConcreteTweener.h"
#include "../Core/THGameObject.h"
#include "../Core/THSprite.h"

namespace THEngine
{
	Delay::Delay(int duration)
	{
		this->duration = duration;
	}

	Delay::~Delay()
	{
	}

	void Delay::OnStart()
	{
		TweenUnit::OnStart();

		tweener = Ptr<EmptyTweener>::New(duration).Get();
	}

	/////////////////////////////////////////////////////
	MoveTo::MoveTo(Vector3f  position, int duration, Tweener::Type type)
	{
		this->position = position;
		this->duration = duration;
		this->type = type;
	}

	MoveTo::~MoveTo()
	{
	}

	void MoveTo::OnStart()
	{
		TweenUnit::OnStart();

		tweener = Ptr<Vector3fTweener>::New(&object->position, object->position, position, duration, type).Get();
	}

	////////////////////////////////////////////////////
	MoveBy::MoveBy(Vector3f  position, int duration, Tweener::Type type)
	{
		this->position = position;
		this->duration = duration;
		this->type = type;
	}

	MoveBy::~MoveBy()
	{
	}

	void MoveBy::OnStart()
	{
		TweenUnit::OnStart();

		tweener = Ptr<Vector3fTweener>::New(&object->position, object->position, object->position + position, duration, type).Get();
	}

	/////////////////////////////////////////////////////////
	FadeTo::FadeTo(float alpha, int duration, Tweener::Type type)
	{
		this->alpha = alpha;
		this->duration = duration;
		this->type = type;
	}

	FadeTo::~FadeTo()
	{
	}

	void FadeTo::OnStart()
	{
		TweenUnit::OnStart();

		tweener = Ptr<FloatTweener>::New(&object->alpha, object->alpha, alpha, duration, type).Get();
	}

	/////////////////////////////////////////////////////////
	FadeOut::FadeOut(int duration, Tweener::Type type)
	{
		this->duration = duration;
		this->type = type;
	}

	FadeOut::~FadeOut()
	{
	}

	void FadeOut::OnStart()
	{
		TweenUnit::OnStart();

		tweener = Ptr<FloatTweener>::New(&object->alpha, object->alpha, 0, duration, type).Get();
	}

	void FadeOut::OnFinish()
	{
		TweenUnit::OnFinish();

		object->MarkDestroy();
	}

	///////////////////////////////////////////////////////////
	ColorTo::ColorTo(Vector3f color, int duration, Tweener::Type type)
	{
		this->color = color;
		this->duration = duration;
		this->type = type;
	}

	ColorTo::~ColorTo()
	{
	}

	void ColorTo::OnStart()
	{
		TweenUnit::OnStart();

		tweener = Ptr<Vector3fTweener>::New(&object->color, object->color, color, duration, type).Get();
	}

	///////////////////////////////////////////////////////////
	ScaleTo::ScaleTo(Vector3f scale, int duration, Tweener::Type type)
	{
		this->scale = scale;
		this->duration = duration;
		this->type = type;
	}

	ScaleTo::~ScaleTo()
	{
	}

	void ScaleTo::OnStart()
	{
		TweenUnit::OnStart();

		tweener = Ptr<Vector3fTweener>::New(&object->scale, object->scale, scale, duration, type).Get();
	}

	///////////////////////////////////////////////////////////
	Rotate2D::Rotate2D(float rotation, int duration, Tweener::Type type)
	{
		this->rotation = rotation;
		this->duration = duration;
		this->type = type;
	}

	Rotate2D::~Rotate2D()
	{
	}

	void Rotate2D::OnStart()
	{
		TweenUnit::OnStart();

		tweener = Ptr<FloatTweener>::New(&((Sprite*)object)->rotation, ((Sprite*)object)->rotation,
			((Sprite*)object)->rotation + rotation, duration, type).Get();
	}

	///////////////////////////////////////////////////////////
	SpeedTo::SpeedTo(float speed, int duration, Tweener::Type type)
	{
		this->speed = speed;
		this->duration = duration;
		this->type = type;
	}

	SpeedTo::~SpeedTo()
	{
	}

	void SpeedTo::OnStart()
	{
		TweenUnit::OnStart();

		tweener = Ptr<FloatTweener>::New(&((Sprite*)object)->speed, ((Sprite*)object)->speed,
			speed, duration, type).Get();
	}
}