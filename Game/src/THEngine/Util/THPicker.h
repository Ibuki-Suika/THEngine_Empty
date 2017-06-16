#ifndef TH_PICKER_H
#define TH_PICKER_H

#include <Common\THCommon.h>
#include <Math\THVector.h>

namespace THEngine
{
	class Camera3D;

	class Picker : public Object
	{
	private:
		Camera3D* camera = nullptr;
		int width;
		int height;

		Vector3f front;
		Vector3f up;
		Vector3f right;
		float fovScale;
		float whFactor;

	public:
		Picker();
		Picker(Camera3D* camera);
		virtual ~Picker();

		Vector3f GenerateRay(int x, int y);
	};
}

#endif
