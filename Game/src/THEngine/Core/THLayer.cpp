#include "THLayer.h"
#include "THGame.h"

using namespace THEngine;

Layer::Layer()
{
	this->left = 0;
	this->top = 0;
	this->width = Game::GetInstance()->GetWidth();
	this->height = Game::GetInstance()->GetHeight();

	Camera2D* camera2D = new Camera2D();
	camera2D->SetWidth(width);
	camera2D->SetHeight(height);
	camera2D->SetPosition(Vector3f(width / 2, height / 2, 0));
	SetCamera(camera2D);
}

Layer::Layer(int left, int top, int width, int height)
{
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;

	Camera2D* camera2D = new Camera2D();
	camera2D->SetWidth(width);
	camera2D->SetHeight(height);
	camera2D->SetPosition(Vector3f(width / 2, height / 2, 0));
	SetCamera(camera2D);
}

Layer::~Layer()
{
	TH_SAFE_RELEASE(camera);
}

void Layer::AddChild(GameObject* obj)
{
	rootNode.AddChild(obj);
}

void Layer::SetCamera(Camera* camera)
{
	TH_SAFE_RELEASE(this->camera);
	this->camera = camera;
	this->camera->Retain();
}

void Layer::Update()
{
	EngineObject::Update();

	rootNode.Update();
}

void Layer::Draw()
{
	auto app = Application::GetInstance();
	app->SetViewport(left, top, width, height);

	Camera2D* camera2D;
	Camera3D* camera3D;
	if (camera2D = dynamic_cast<Camera2D*>(camera)){
		Vector3f pos = camera2D->GetPosition();
		app->SetOrtho(pos.x - camera2D->GetWidth() / 2, pos.y - camera2D->GetHeight() / 2,
			camera2D->GetWidth(), camera2D->GetHeight(), 0, TH_MAX_Z);

		D3DXMATRIX matrix;
		D3DXMatrixIdentity(&matrix);
		app->SetViewTransform(&matrix);
	}
	else if (camera3D = dynamic_cast<Camera3D*>(camera))
	{
		D3DXMATRIX matrix;
		D3DXMatrixPerspectiveFovLH(&matrix, ToRad(camera3D->fov), (float)width / height, 0.1f, 10000.0f);
		app->SetProjectionTransform(&matrix);

		D3DXMatrixLookAtLH(&matrix, &D3DXVECTOR3(camera3D->position.x, camera3D->position.y, camera3D->position.z),
			&D3DXVECTOR3(camera3D->lookAt.x, camera3D->lookAt.y, camera3D->lookAt.z), 
			&D3DXVECTOR3(camera3D->up.x, camera3D->up.y, camera3D->up.z));
		app->SetViewTransform(&matrix);
	}

	rootNode.Visit();

	SetupRenderState();

	Game::GetInstance()->Render();

	Application::GetInstance()->ClearDepthBuffer();
}

void Layer::SetupRenderState()
{
	auto app = Application::GetInstance();
	auto renderState = app->GetRenderState();

	renderState->fogEnable = this->fogEnable;
	renderState->fog = this->fog;

	renderState->lightingEnable = lightingEnable;

	auto iter = lights.GetIterator();
	while (iter->HasNext())
	{
		auto light = iter->Next();
		switch (light->GetType())
		{
		case Light::DIRECTIONAL:
			renderState->directionalLights.Add((DirectionalLight*)light);
			break;
		}
	}
}

void Layer::DestroyObjectImmediately(GameObject* obj)
{
	obj->OnDestroy();
	rootNode.RemoveChild(obj);
}

void Layer::OnActivate()
{
	activated = true;
}

void Layer::OnDestroy()
{
	rootNode.OnDestroy();
}