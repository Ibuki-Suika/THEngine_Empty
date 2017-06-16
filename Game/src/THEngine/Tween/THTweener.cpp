#include "THTweener.h"

namespace THEngine
{
	Tweener::Tweener()
	{

	}

	Tweener::~Tweener()
	{

	}

	void Tweener::Update()
	{
		elapsed++;
	}

	//给定一个（0,1）区间的参数t，根据不同的type，返回不同的（0,1）区间的值
	float Tweener::Tween(float t, Type type)
	{
		switch (type)
		{
		case SIMPLE:     //线性函数
			return t;
		case EASE_IN:
			return t * t;
		case EASE_OUT:
			return t * (2 - t);
		case EASE_INOUT:
			if (t <= 0.5f)
			{
				return t * t * 2;
			}
			return (2 * t - 1)*(3 - 2 * t)*0.5 + 0.5;
		default:
			return t;
		}
	}

	float Tweener::CalcInterpolation(float initialValue, float finalValue, int duration, int elapsed, Type type)
	{
		float t = ((float)elapsed) / duration;
		return initialValue + (finalValue - initialValue) * Tween(t, type);
	}
}