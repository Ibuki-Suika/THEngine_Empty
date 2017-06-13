#include "THCamera.h"
#include "THLayer.h"
#include "THGame.h"
#include <Asset\THAssetManager.h>
#include <Asset\THCubeMap.h>
#include <Asset\THRenderTexture.h>
#include <Platform\THApplication.h>
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
		TH_SAFE_RELEASE(renderTexture);
	}

	void Camera::Update()
	{
		EngineObject::Update();
	}

	void Camera::Draw()
	{

	}
	
	void Camera::SetRenderTexture(RenderTexture* renderTexture)
	{
		TH_SET(this->renderTexture, renderTexture);
	}

	void Camera::SetupViewport(Layer* layer)
	{
		auto app = Application::GetInstance();
		app->SetViewport(layer->GetLeft() + this->viewport.left, layer->GetTop() + this->viewport.top,
			this->viewport.Width(), this->viewport.Height());
	}

	void Camera::Render(Layer* layer)
	{
		auto renderState = Application::GetInstance()->GetRenderState();
		TH_SET(renderState->camera, this);
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

	void Camera2D::Render(Layer* layer)
	{
		Camera::Render(layer);

		auto app = Application::GetInstance();

		SetupViewport(layer);

		Vector3f pos = GetPosition();
		app->SetOrtho(pos.x - GetWidth() / 2, pos.y - GetHeight() / 2, 
			GetWidth(), GetHeight(), this->znear, this->zfar);

		Matrix matrix;
		Matrix::Identity(&matrix);
		app->SetViewMatrix(matrix);

		Game::GetInstance()->GetRenderPipeline()->Render();

		Application::GetInstance()->ClearDepthBuffer();
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

	void Camera3D::Render(Layer* layer)
	{
		Camera::Render(layer);

		auto app = Application::GetInstance();

		SetupViewport(layer);

		if (layer->GetSkyBox())
		{
			Game::GetInstance()->GetRenderPipeline()->GetSkyBoxRenderer()->RenderSkyBox(this, layer);
		}

		Matrix matrix;
		Matrix::Perspective(&matrix, this->fov, (float)this->viewport.Width() / this->viewport.Height(),
			this->znear, this->zfar);

		app->SetProjectionMatrix(matrix);

		Matrix::LookAt(&matrix, Vector3f(this->position.x, this->position.y,  this->position.z),
			Vector3f(this->lookAt.x, this->lookAt.y, this->lookAt.z),
			Vector3f(this->up.x, this->up.y, this->up.z));
		app->SetViewMatrix(matrix);

		Game::GetInstance()->GetRenderPipeline()->Render();

		Application::GetInstance()->ClearDepthBuffer();
	}
}

