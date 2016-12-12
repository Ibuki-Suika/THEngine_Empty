#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <limits>
#include "Matrix.h"

namespace THEngine
{
	const float PI = 3.1415926f;

	float ToAngle(float x, float y);
	float ToRad(float angle);
	float RadToDegree(float rad);

	int Random(int a, int b);  //返回[a,b]范围的随机整数

	Vector2f VecRotate2D(Vector2f pos, float angle);
}

#endif
