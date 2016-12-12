#ifndef THCAMERA_H
#define THCAMERA_H

#include "../Common/THCommon.h"
#include "../Math/MathUtil.h"

namespace THEngine
{
	class Camera : public EngineObject
	{
	protected:
		Vector3f position;

	public:
		inline void SetPosition(Vector3f position) { this->position = position; }
		inline Vector3f GetPosition() { return position; }

		virtual void Update() override;
		virtual void Draw() override;
	};

	class Camera2D : public Camera
	{
	protected:
		float width, height;

	public:
		Camera2D();
		virtual ~Camera2D();

		inline void SetWidth(float width) { this->width = width; }
		inline float GetWidth() { return width; }

		inline void SetHeight(float height) { this->height = height; }
		inline float GetHeight() { return height; }
	};

	class Camera3D : public Camera
	{
	protected:
		
		Vector3f lookAt;
		Vector3f up;
		
		float fov = 60.0f;

	public:
		Camera3D();
		virtual ~Camera3D();

		inline void SetLookAt(Vector3f lookAt) { this->lookAt = lookAt; }
		inline Vector3f GetLookAt() { return lookAt; }

		inline void SetUp(Vector3f up) { this->up = up; }
		inline Vector3f GetUp() { return up; }

		inline void SetFov(float fov) { this->fov = fov; }
		inline float GetFov() { return fov; }

		friend class Layer;
	};
}

#endif