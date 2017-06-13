#ifndef TH_MATH_UTIL_H
#define TH_MATH_UTIL_H

#include <limits>
#include "THVector.h"

namespace THEngine
{
	class RandomGenerator;

	class Math
	{
	private:
		static RandomGenerator* GetRandomGenerator();

	public:
		static const float PI;

		static float ToAngle(float x, float y);
		static float StandardizeAngle(float angle);
		static float ToRad(float angle);
		static float RadToDegree(float rad);

		static int Random(int a, int b);  //返回[a,b]范围的随机整数

		static Vector2f VecRotate2D(const Vector2f& pos, float angle);
	};
}

#endif
