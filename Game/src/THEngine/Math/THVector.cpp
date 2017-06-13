#include "THVector.h"
#include "string.h"
#include "math.h"
#include <Common\THDefine.h>

namespace THEngine
{
	//////////////////  Vector2f  /////////////////////
	Vector2f::Vector2f()
	{
		x = y = 0;
	}

	Vector2f::Vector2f(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float Vector2f::Norm()
	{
		return sqrt(x*x + y*y);
	}

	Vector2f Vector2f::Normalize()
	{
		float norm = Norm();
		ASSERT(Norm() != 0);

		Vector2f ret = *this;
		ret.x /= norm;
		ret.y /= norm;
		return ret;
	}

	Vector2f Vector2f::operator +(const Vector2f& a)
	{
		Vector2f ans;
		ans.x = this->x + a.x;
		ans.y = this->y + a.y;
		return ans;
	}

	Vector2f Vector2f::operator -(const Vector2f& a)
	{
		Vector2f ans;
		ans.x = this->x - a.x;
		ans.y = this->y - a.y;
		return ans;
	}

	Vector2f Vector2f::operator -()
	{
		Vector2f ans;
		ans.x = -this->x;
		ans.y = -this->y;
		return ans;
	}

	bool Vector2f::operator ==(const Vector2f& a)
	{
		float ex = fabs(x - a.x);
		float ey = fabs(y - a.y);
		return ex < 1e-4 && ey < 1e-4;
	}

	bool Vector2f::operator !=(const Vector2f& a)
	{
		return !(*this == a);
	}

	///////////////////////  Vector3f   ///////////////////////

	Vector3f::Vector3f()
	{
		x = y = z = 0;
	}

	Vector3f::Vector3f(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float Vector3f::Norm() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	Vector3f Vector3f::Cross(const Vector3f& a, const Vector3f& b)
	{
		Vector3f v;
		v.x = a.z*b.y - a.y*b.z;
		v.y = a.x*b.z - a.z*b.x;
		v.z = a.y*b.x - a.x*b.y;
		return v;
	}

	float Vector3f::Dot(const Vector3f& a, const Vector3f& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	Vector3f Vector3f::operator*(float a) const
	{
		Vector3f ret;
		ret.x = x * a;
		ret.y = y * a;
		ret.z = z * a;
		return ret;
	}

	Vector3f operator*(float a, const Vector3f& v)
	{
		return v * a;
	}

	Vector3f Vector3f::Normalize() const
	{
		float norm = Norm();
		ASSERT(Norm() != 0);

		Vector3f ret = *this;
		ret.x /= norm;
		ret.y /= norm;
		ret.z /= norm;
		return ret;
	}

	Vector3f Vector3f::operator +(const Vector3f& a) const
	{
		Vector3f v;
		v.x = this->x + a.x;
		v.y = this->y + a.y;
		v.z = this->z + a.z;
		return v;
	}

	Vector3f Vector3f::operator -(const Vector3f& a) const
	{
		Vector3f v;
		v.x = this->x - a.x;
		v.y = this->y - a.y;
		v.z = this->z - a.z;
		return v;
	}

	Vector3f Vector3f::operator -() const
	{
		Vector3f v;
		v.x = -this->x;
		v.y = -this->y;
		v.z = -this->z;
		return v;
	}

	Vector3f& Vector3f::operator +=(const Vector3f& a)
	{
		this->x += a.x;
		this->y += a.y;
		this->z += a.z;
		return *this;
	}

	Vector3f& Vector3f::operator -=(const Vector3f& a)
	{
		this->x -= a.x;
		this->y -= a.y;
		this->z -= a.z;
		return *this;
	}

	Vector3f& Vector3f::operator *=(const Vector3f& a)
	{
		this->x *= a.x;
		this->y *= a.y;
		this->z *= a.z;
		return *this;
	}

	Vector3f& Vector3f::operator /=(const Vector3f& a)
	{
		this->x /= a.x;
		this->y /= a.y;
		this->z /= a.z;
		return *this;
	}

	bool Vector3f::operator ==(const Vector3f& a)
	{
		float ex = fabs(x - a.x);
		float ey = fabs(y - a.y);
		float ez = fabs(z - a.z);
		return ex < 1e-4 && ey < 1e-4 &&ez < 1e-4;
	}

	bool Vector3f::operator !=(const Vector3f& a)
	{
		return !(*this == a);
	}

	/*************************   Vector4f   ********************************/
	Vector4f::Vector4f()
	{
		memset(_data, 0, sizeof(_data));
	}

	Vector4f::Vector4f(float a1, float a2, float a3, float a4)
	{
		_data[0] = a1;
		_data[1] = a2;
		_data[2] = a3;
		_data[3] = a4;
	}

	float& Vector4f::operator [](const int& index)
	{
		return _data[index];
	}
}




