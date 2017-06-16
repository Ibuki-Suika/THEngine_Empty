#include "THLayer.h"
#include "THGame.h"
#include "THDataStack.h"
#include <Asset\THCubeMap.h>
#include <Platform\THDevice.h>

namespace THEngine
{
	Layer::Layer() : Layer(0, 0, Game::GetInstance()->GetWidth(), Game::GetInstance()->GetHeight())
	{
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
		camera2D->SetViewport(RectInt(0, this->width, 0, this->height));
		AddCamera(camera2D);
	}

	Layer::~Layer()
	{
		TH_SAFE_RELEASE(skyBox);
	}

	void Layer::AddChild(GameObject* obj)
	{
		rootNode.AddChild(obj);
	}

	void Layer::AddCamera(Camera* camera)
	{
		cameraList.Add(camera);
		if (camera->viewport.left == 0 && camera->viewport.right == 0
			&& camera->viewport.top == 0 && camera->viewport.bottom == 0)
		{
			camera->viewport.right = this->width;
			camera->viewport.bottom = this->height;
		}
	}

	Camera* Layer::GetCameraByName(const String& name)
	{
		auto iter = cameraList.GetIterator();
		while (iter->HasNext())
		{
			auto camera = iter->Next();
			if (camera->GetName() == name)
			{
				return camera;
			}
		}
		return nullptr;
	}

	Camera* Layer::GetCameraByIndex(int index)
	{
		int i = 0;
		auto iter = cameraList.GetIterator();
		while (iter->HasNext())
		{
			auto camera = iter->Next();
			if (index == i)
			{
				return camera;
			}
			i++;
		}
		return nullptr;
	}

	Camera* Layer::GetFirstCamera()
	{
		return GetCameraByIndex(0);
	}

	void Layer::SetCameraByName(Camera* camera, const String& name)
	{
		auto iter = cameraList.GetIterator();
		while (iter->HasNext())
		{
			auto curCamera = iter->Next();
			if (curCamera->GetName() == name)
			{
				iter->Remove();
				iter->AddAfter(camera);
				if (camera->viewport.left == 0 && camera->viewport.right == 0
					&& camera->viewport.top == 0 && camera->viewport.bottom == 0)
				{
					camera->viewport.right = this->width;
					camera->viewport.bottom = this->height;
				}
				return;
			}
		}
	}

	void Layer::SetCameraByIndex(Camera* camera, int index)
	{
		int i = 0;
		auto iter = cameraList.GetIterator();
		while (iter->HasNext())
		{
			iter->Next();
			if (index == i)
			{
				iter->Remove();
				iter->AddAfter(camera);
				if (camera->viewport.left == 0 && camera->viewport.right == 0
					&& camera->viewport.top == 0 && camera->viewport.bottom == 0)
				{
					camera->viewport.right = this->width;
					camera->viewport.bottom = this->height;
				}
				return;
			}
			i++;
		}
	}

	void Layer::SetFirstCamera(Camera* camera)
	{
		SetCameraByIndex(camera, 0);
	}

	void Layer::SetSkyBox(CubeMap* skyBox)
	{
		TH_SET(this->skyBox, skyBox);
	}

	void Layer::Update()
	{
		EngineObject::Update();

		for (auto iter = cameraList.GetIterator(); iter->HasNext();)
		{
			iter->Next()->Update();
		}

		rootNode.Update();
	}

	void Layer::Draw()
	{
		auto iter = cameraList.GetIterator();
		while (iter->HasNext())
		{
			auto camera = iter->Next();

			DataStack::GetInstance()->Reset();
			rootNode.Visit();

			SetupRenderState();

			camera->Render(this);
		}
	}

	void Layer::OnLoad(AsyncInfo* info)
	{
		EngineObject::OnLoad(info);

		rootNode.OnLoad(info);
	}

	void Layer::SetupRenderState()
	{
		auto device = Device::GetInstance();
		auto renderState = device->GetRenderState();

		renderState->environment = &this->environment;
	}

	void Layer::DestroyObjectImmediately(GameObject* obj)
	{
		obj->OnDestroy();
		rootNode.RemoveChild(obj);
	}

	void Layer::OnDestroy()
	{
		rootNode.OnDestroy();
	}

	void Layer::Clear()
	{
		rootNode.RemoveAllChildren();
		environment.Clear();
	}
}