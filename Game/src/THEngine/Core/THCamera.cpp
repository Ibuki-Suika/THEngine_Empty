#include "THCamera.h"
#include "THLayer.h"
#include "THGame.h"
#include <Asset\THAssetManager.h>
#include <Asset\THCubeMap.h>
#include <Asset\THRenderTexture.h>
#include <Platform\THDevice.h>
#include <Renderer\THRenderPipeline.h>
#include <Renderer\THSkyBoxRenderer.h>

namespace THEngine
{
	Camera::Camera()
	{
	}

	Camera::Camera(const String& name) : name(name)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::Update()
	{
		EngineObject::Update();
	}

	void Camera::Draw()
	{
	}

	void Camera::SetRenderTexture(Ptr<RenderTexture> renderTexture)
	{
		this->renderTexture = renderTexture;
	}

	void Camera::SetupViewport(Ptr<Layer> layer)
	{
		auto device = Device::GetInstance();
		device->SetViewport(layer->GetLeft() + this->viewport.left, layer->GetTop() + this->viewport.top,
			this->viewport.Width(), this->viewport.Height());
	}

	void Camera::Render(Ptr<Layer> layer)
	{
		auto renderState = Device::GetInstance()->GetRenderState();
		renderState->camera = this;
	}

	/////////////////////////////////////////
	Camera2D::Camera2D()
	{
		this->znear = 0;
	}

	Camera2D::Camera2D(const String& name) : Camera(name)
	{
		this->znear = 0;
	}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::Render(Ptr<Layer> layer)
	{
		Camera::Render(layer);

		auto device = Device::GetInstance();

		SetupViewport(layer);

		Vector3f pos = GetPosition();
		device->SetOrtho(pos.x - GetWidth() / 2, pos.y - GetHeight() / 2,
			GetWidth(), GetHeight(), this->znear, this->zfar);

		Matrix matrix;
		Matrix::Identity(&matrix);
		device->SetViewMatrix(matrix);

		Game::GetInstance()->GetRenderPipeline()->Render();

		Device::GetInstance()->ClearDepthBuffer();
	}

	/////////////////////////////////////////
	Camera3D::Camera3D()
	{
	}

	Camera3D::Camera3D(const String& name) : Camera(name)
	{
	}

	Camera3D::~Camera3D()
	{
	}

	void Camera3D::Render(Ptr<Layer> layer)
	{
		Camera::Render(layer);

		auto device = Device::GetInstance();

		SetupViewport(layer);

		if (layer->GetSkyBox() != nullptr)
		{
			Game::GetInstance()->GetRenderPipeline()->GetSkyBoxRenderer()->RenderSkyBox(this, layer);
		}

		Matrix matrix;
		Matrix::Perspective(&matrix, this->fov, (float)this->viewport.Width() / this->viewport.Height(),
			this->znear, this->zfar);

		device->SetProjectionMatrix(matrix);

		Matrix::LookAt(&matrix, Vector3f(this->position.x, this->position.y, this->position.z),
			Vector3f(this->lookAt.x, this->lookAt.y, this->lookAt.z),
			Vector3f(this->up.x, this->up.y, this->up.z));
		device->SetViewMatrix(matrix);

		Game::GetInstance()->GetRenderPipeline()->Render();

		device->ClearDepthBuffer();
	}
}