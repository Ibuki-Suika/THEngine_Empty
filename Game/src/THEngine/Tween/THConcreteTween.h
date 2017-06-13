#ifndef THCONCRETETWEEN_H
#define THCONCRETETWEEN_H

#include "THTween.h"
#include "THTweener.h"
#include <Math\THVector.h>

namespace THEngine
{
	class Delay : public TweenUnit
	{
	public:
		Delay(int duration);
		virtual ~Delay();

		virtual void OnStart() override;
	};

	/////////////////////////////////////////////////////
	class MoveTo : public TweenUnit
	{
	protected:
		Vector3f position;
		Tweener::Type type;

	public:
		MoveTo(Vector3f position, int duration, Tweener::Type type);
		virtual ~MoveTo();

		virtual void OnStart() override;
	};

	////////////////////////////////////////////////////
	class MoveBy : public TweenUnit
	{
	protected:
		Vector3f position;
		Tweener::Type type;

	public:
		MoveBy(Vector3f position, int duration, Tweener::Type type);
		virtual ~MoveBy();

		virtual void OnStart() override;
	};

	////////////////////////////////////////////////////
	class FadeTo : public TweenUnit
	{
	protected:
		float alpha;
		Tweener::Type type;

	public:
		FadeTo(float alpha, int duration, Tweener::Type type);
		virtual ~FadeTo();

		virtual void OnStart() override;
	};

	////////////////////////////////////////////////////
	class FadeOut : public TweenUnit
	{
	protected:
		Tweener::Type type;

	public:
		FadeOut(int duration, Tweener::Type type);
		virtual ~FadeOut();

		virtual void OnStart() override;
		virtual void OnFinish() override;
	};

	/////////////////////////////////////////////////////
	class ColorTo : public TweenUnit
	{
	protected:
		Vector3f color;
		Tweener::Type type;

	public:
		ColorTo(Vector3f color, int duration, Tweener::Type type);
		virtual ~ColorTo();

		virtual void OnStart() override;
	};

	/////////////////////////////////////////////////////
	class ScaleTo : public TweenUnit
	{
	protected:
		Vector3f scale;
		Tweener::Type type;

	public:
		ScaleTo(Vector3f scale, int duration, Tweener::Type type);
		virtual ~ScaleTo();

		virtual void OnStart() override;
	};

	/////////////////////////////////////////////////////
	class Rotate2D : public TweenUnit
	{
	protected:
		float rotation;
		Tweener::Type type;

	public:
		Rotate2D(float rotation, int duration, Tweener::Type type);
		virtual ~Rotate2D();

		virtual void OnStart() override;
	};

	/////////////////////////////////////////////////////
	class SpeedTo : public TweenUnit
	{
	protected:
		float speed;
		Tweener::Type type;

	public:
		SpeedTo(float rotation, int duration, Tweener::Type type);
		virtual ~SpeedTo();

		virtual void OnStart() override;
	};
}

#endif