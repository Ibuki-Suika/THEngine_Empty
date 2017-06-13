#include "THQuaternion.h"
#include "THMathUtil.h"

namespace THEngine
{
	Quaternion::Quaternion()
	{
		D3DXQuaternionIdentity(&this->quaternion);
	}

	void Quaternion::Identity(Quaternion* quaternion)
	{
		D3DXQuaternionIdentity(&quaternion->quaternion);
	}

	void Quaternion::RotateAngleAxis(Quaternion* quaternion, const Vector3f& axis, float degree)
	{
		D3DXQuaternionRotationAxis(&quaternion->quaternion, &D3DXVECTOR3(axis.x, axis.y, axis.z), Math::ToRad(degree));
	}

	Quaternion& Quaternion::operator*=(const Quaternion& quaternion)
	{
		this->quaternion *= quaternion.quaternion;
		return *this;
	}
}