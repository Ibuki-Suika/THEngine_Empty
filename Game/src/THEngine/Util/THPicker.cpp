#include "THPicker.h"
#include <Core\THCamera.h>

namespace THEngine
{
	Picker::Picker()
	{
	}

	Picker::Picker(Ptr<Camera3D> camera)
	{
		this->camera = camera;
		this->width = camera->GetViewport().Width();
		this->height = camera->GetViewport().Height();
		this->front = (camera->GetLookAt() - camera->GetPosition()).Normalize();
		this->up = camera->GetUp().Normalize();
		this->right = Vector3f::Cross(front, up);
		this->fovScale = tan(Math::ToRad(camera->GetFov() * 0.5)) * 2;
		this->whFactor = this->width * 1.0f / this->height;
	}

	Picker::~Picker()
	{
	}

	Vector3f Picker::GenerateRay(int x, int y)
	{
		float sx = x / this->width;
		float sy = y / this->height;

		Vector3f r = right * ((sx - 0.5) * fovScale) * this->whFactor;
		Vector3f u = up * ((sy - 0.5) * fovScale);

		return (r + u + front).Normalize();
	}
}