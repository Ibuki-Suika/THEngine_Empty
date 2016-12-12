#ifndef THRENDERQUEUE_H
#define THRENDERQUEUE_H

#include "../Core/THGameObject.h"

namespace THEngine
{
	class RenderQueue : public Object
	{
	protected:
		ArrayList<GameObject*> objList;

	public:
		virtual void Add(GameObject* obj);
		void Clear();
		virtual void Render();
	};

	class SpriteRenderQueue : public RenderQueue
	{
	public:
		virtual void Add(GameObject* obj) override;
		virtual void Render() override;

		static bool Compare(GameObject* sprite1, GameObject* sprite2);
	};

	class NormalRenderQueue : public RenderQueue
	{
	public:
		NormalRenderQueue();
		virtual ~NormalRenderQueue();
	};
}

#endif