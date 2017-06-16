#include "THMathUtil.h"
#include "THRandomGenerator.h"

namespace THEngine
{
	const float Math::PI = 3.1415926f;

	float Math::ToAngle(float x, float y)
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
		return angle >= 0 ? angle : angle + 360;
	}

	float Math::StandardizeAngle(float angle)
	{
		angle -= ((int)angle) / 360 * 360;
		return angle >= 0 ? angle : angle + 360;
	}

	float Math::RadToDegree(float rad)
	{
		return rad * 180.0f / PI;
	}

	float Math::ToRad(float angle)
	{
		return angle * PI / 180.0f;
	}

	int Math::Random(int a, int b)
	{
		return GetRandomGenerator()->Next(a, b);
	}

	Vector2f Math::VecRotate2D(const Vector2f& pos, float angle)
	{
		Vector2f ret;
		float rad = ToRad(angle);

		ret.x = cos(rad) * pos.x - sin(rad) * pos.y;
		ret.y = sin(rad) * pos.x + cos(rad) * pos.y;

		return ret;
	}

	RandomGenerator* Math::GetRandomGenerator()
	{
		static RandomGenerator randomGenerator(time(NULL));
		return &randomGenerator;
	}
}