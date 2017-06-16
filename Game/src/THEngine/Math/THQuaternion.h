#ifndef TH_QUATERNION_H
#define TH_QUATERNION_H

#include <Common\THCommon.h>
#include "THVector.h"

namespace THEngine
{
	class Quaternion
	{
	private:
		D3DXQUATERNION quaternion;

	public:
		Quaternion();

		static void Identity(Quaternion* quaternion);
		static void RotateAngleAxis(Quaternion* quaternion, const Vector3f& axis, float degree);

		Quaternion& operator *=(const Quaternion& quaternion);

		friend class Matrix;
	};
}

#endif