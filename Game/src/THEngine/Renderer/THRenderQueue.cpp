#include "THRenderQueue.h"
#include "THRenderer.h"
#include "../Core/THGame.h"

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
	return ((Sprite*)(sprite1))->GetPosition().z > ((Sprite*)(sprite2))->GetPosition().z;
}

void SpriteRenderQueue::Render()
{
	SpriteRenderer* renderer = Game::GetInstance()->GetSpriteRenderer();

	objList.Sort(0, objList.Size(), SpriteRenderQueue::Compare);

	auto iter = objList.GetIterator();
	while (iter->HasNext())
	{
		iter->Next()->Draw();
	}

}

void SpriteRenderQueue::Add(GameObject* obj)
{
	objList.Add(obj);
}

/////////////////////////////////////////////
NormalRenderQueue::NormalRenderQueue()
{

}

NormalRenderQueue::~NormalRenderQueue()
{

}