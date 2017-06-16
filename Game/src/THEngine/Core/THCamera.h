#ifndef THCAMERA_H
#define THCAMERA_H

#include <Common\THCommon.h>
#include <Math\THMathUtil.h>
#include <Math\THQuaternion.h>

namespace THEngine
{
	class Layer;
	class RenderTexture;

	class Camera : public EngineObject
	{
	protected:
		Layer* layer = nullptr;

		Vector3f position;
		String name = "";
		RenderTexture* renderTexture = nullptr;
		RectInt viewport = RectInt(0, 0, 0, 0);
		float znear = 1;
		float zfar = TH_MAX_Z;

	protected:
		void SetupViewport(Layer* layer);

	public:
		Camera();
		Camera(const String& name);
		virtual ~Camera();

		inline void SetPosition(const Vector3f& position) { this->position = position; }
		inline const Vector3f& GetPosition() { return position; }

		virtual void Update() override;
		virtual void Draw() override;

		inline void SetName(const String& name) { this->name = name; }
		inline String& GetName() { return name; }

		void SetRenderTexture(RenderTexture* renderTexture);

		inline void SetViewport(const RectInt& rect) { this->viewport = rect; }
		inline const RectInt& GetViewport() const { return this->viewport; }

		inline void SetZNear(float znear) { this->znear = znear; }
		inline float GetZNear() const { return this->znear; }

		inline void SetZFar(float zfar) { this->zfar = zfar; }
		inline float GetZFar() const { return this->zfar; }

		virtual bool Is2D() = 0;

		virtual void Render(Layer* layer);

		friend class Layer;
	};

	class Camera2D : public Camera
	{
	protected:
		float width, height;

	public:
		Camera2D();
		Camera2D(const String& name);
		virtual ~Camera2D();

		inline void SetWidth(float width) { this->width = width; }
		inline float GetWidth() { return width; }

		inline void SetHeight(float height) { this->height = height; }
		inline float GetHeight() { return height; }

		virtual bool Is2D() override final { return true; }

		virtual void Render(Layer* layer) override;
	};

	class Camera3D : public Camera
	{
	protected:
		Vector3f lookAt;
		Vector3f up;

		float fov = 60.0f;

		Quaternion rotation3D;

	public:
		Camera3D();
		Camera3D(const String& name);
		virtual ~Camera3D();

		inline void SetLookAt(const Vector3f& lookAt) { this->lookAt = lookAt; }
		inline const Vector3f& GetLookAt() { return lookAt; }

		inline void SetUp(const Vector3f& up) { this->up = up; }
		inline const Vector3f& GetUp() { return up; }

		inline void SetFov(float fov) { this->fov = fov; }
		inline float GetFov() { return fov; }

		virtual bool Is2D() override final { return false; }

		virtual void Render(Layer* layer) override;

		friend class Layer;
	};
}

#endif