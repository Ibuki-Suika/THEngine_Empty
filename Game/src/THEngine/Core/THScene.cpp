#include "THScene.h"
#include <Platform\THDevice.h>

namespace THEngine
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		auto iter = layers.GetIterator();
		while (iter->HasNext())
		{
			Ptr<Layer> curLayer = iter->Next();
			curLayer->OnDestroy();
			iter->Remove();
		}
	}

	void Scene::AddLayer(Ptr<Layer> layer)
	{
		auto iter = layers.GetIterator();
		while (iter->HasNext())
		{
			Ptr<Layer> curLayer = iter->Next();
			if (curLayer->order < layer->order)
			{
				iter->AddBefore(layer);
				return;
			}
		}
		iter->AddAfter(layer);
	}

	void Scene::Update()
	{
		EngineObject::Update();

		auto iter = layers.GetIterator();
		while (iter->HasNext())
		{
			Ptr<Layer> curLayer = iter->Next();
			if (curLayer->IsPaused() == false)
			{
				curLayer->Update();
			}
		}
	}

	void Scene::Draw()
	{
		auto iter = layers.GetIterator();
		while (iter->HasNext())
		{
			Ptr<Layer> curLayer = iter->Next();
			curLayer->Draw();
			Device::GetInstance()->GetRenderState()->Clear();
		}
	}

	void Scene::OnLoad(Ptr<AsyncInfo> info)
	{
		EngineObject::OnLoad(info);

		auto iter = layers.GetIterator();
		while (iter->HasNext())
		{
			Ptr<Layer> curLayer = iter->Next();
			curLayer->OnLoad(info);
		}
	}

	void Scene::OnDestroy()
	{
	}
}