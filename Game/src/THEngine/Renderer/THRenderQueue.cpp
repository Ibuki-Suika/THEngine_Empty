#include "THRenderQueue.h"
#include "THRenderer.h"
#include <Core\THGame.h>
#include <Core\THSprite.h>
#include <iostream>

using namespace THEngine;

void RenderQueue::Add(GameObject* obj)
{
	objList.Add(obj);
}

void RenderQueue::Render()
{
	auto iter = objList.GetIterator();
	while (iter->HasNext())
	{
		iter->Next()->Draw();
	}
}

void RenderQueue::Clear()
{
	objList.Clear();
}

/////////////////////////////////////////////

bool SpriteRenderQueue::Compare(GameObject* sprite1, GameObject* sprite2)
{
	Sprite* sp1 = (Sprite*)sprite1;
	Sprite* sp2 = (Sprite*)sprite2;
	if (sp1->GetPosition().z != sp2->GetPosition().z)
	{
		return sp1->GetPosition().z > sp2->GetPosition().z;
	}
	return sp1->GetTexture().Get() > sp2->GetTexture().Get();
}

void SpriteRenderQueue::Render()
{
	objList.Sort(0, objList.Size(), SpriteRenderQueue::Compare);

	RenderQueue::Render();
}

/////////////////////////////////////////////
NormalRenderQueue::NormalRenderQueue()
{

}

NormalRenderQueue::~NormalRenderQueue()
{

}

/////////////////////////////////////////////
GlobalRenderQueue::GlobalRenderQueue()
{

}

GlobalRenderQueue::~GlobalRenderQueue()
{

}

void GlobalRenderQueue::Render()
{

}