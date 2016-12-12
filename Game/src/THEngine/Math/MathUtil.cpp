#include "MathUtil.h"

using namespace THEngine;

namespace THEngine
{
	float ToAngle(float x, float y)
	{
		float angle;
		if (x == 0)
		{
			if (y > 0)
			{
				angle = 90;
			}
			else if (y < 0)
			{
				angle = -90;
			}
			else
			{
				angle = 0;
			}
		}
		else
		{
			angle = atan(y / x) * 180.0f / PI;
		}
		if (x < 0)
		{
			angle = angle + 180;
		}
		return angle;
	}

	float RadToDegree(float rad)
	{
		return rad * 180.0f / PI;
	}

	float ToRad(float angle)
	{
		return angle * PI / 180.0f;
	}

	int Random(int a, int b)
	{
		int temp = rand();
		temp %= b - a + 1;
		return (int)temp + a;
	}

	Vector2f VecRotate2D(Vector2f pos, float angle)
	{
		Vector2f ret;
		float rad = ToRad(angle);

		ret.x = cos(rad) * pos.x - sin(rad) * pos.y;
		ret.y = sin(rad) * pos.x + cos(rad) * pos.y;

		return ret;
	}
}
