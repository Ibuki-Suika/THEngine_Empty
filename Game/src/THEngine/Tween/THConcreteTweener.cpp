#include "THConcreteTweener.h"

namespace THEngine
{
	EmptyTweener::EmptyTweener(int duration)
	{
		this->duration = duration;
	}

	EmptyTweener::~EmptyTweener()
	{

	}

	void EmptyTweener::Update()
	{
		Tweener::Update();
	}

	////////////////////////////////////////////
	FloatTweener::FloatTweener(float* valueToChange, float initValue, float finalValue, int duration, Type type)
	{
		this->valueToChange = valueToChange;
		this->initValue = initValue;
		this->finalValue = finalValue;
		this->duration = duration;
		this->type = type;
	}

	FloatTweener::~FloatTweener()
	{

	}

	void FloatTweener::Update()
	{
		Tweener::Update();

		*valueToChange = CalcInterpolation(initValue, finalValue, duration, elapsed, type);
	}

	///////////////////////////////////////////
	Vector3fTweener::Vector3fTweener(Vector3f* valueToChange, Vector3f initValue, Vector3f finalValue, int duration, Type type)
	{
		this->valueToChange = valueToChange;
		this->initValue = initValue;
		this->finalValue = finalValue;
		this->duration = duration;
		this->type = type;
	}

	Vector3fTweener::~Vector3fTweener()
	{

	}

	void Vector3fTweener::Update()
	{
		Tweener::Update();

		valueToChange->x = CalcInterpolation(initValue.x, finalValue.x, duration, elapsed, type);
		valueToChange->y = CalcInterpolation(initValue.y, finalValue.y, duration, elapsed, type);
		valueToChange->z = CalcInterpolation(initValue.z, finalValue.z, duration, elapsed, type);
	}
}