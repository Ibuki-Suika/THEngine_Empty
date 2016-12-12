#ifndef THCONCRETEWEENER_H
#define THCONCRETEWEENER_H

#include "THTweener.h"

namespace THEngine
{
	class EmptyTweener : public Tweener
	{
	public:
		EmptyTweener(int duration);
		virtual ~EmptyTweener();

		virtual void Update() override;
	};

	///////////////////////////////////////////////
	class FloatTweener : public Tweener
	{
	protected:
		float* valueToChange = nullptr;
		float initValue;
		float finalValue;

	public:
		FloatTweener(float* valueToChange, float initValue, float finalValue, int duration, Type type);
		virtual ~FloatTweener();

		virtual void Update() override;
	};

	///////////////////////////////////////////////
	class Vector3fTweener : public Tweener
	{
	protected:
		Vector3f* valueToChange = nullptr;
		Vector3f initValue;
		Vector3f finalValue;

	public:
		Vector3fTweener(Vector3f* valueToChange, Vector3f initValue, Vector3f finalValue, int duration, Type type);
		virtual ~Vector3fTweener();

		virtual void Update() override;
	};
}

#endif