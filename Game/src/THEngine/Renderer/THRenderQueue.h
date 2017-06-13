#ifndef THRENDERQUEUE_H
#define THRENDERQUEUE_H

#include <Core\THGameObject.h>

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

		inline ArrayList<GameObject*>* GetObjects() { return &this->objList; }
	};

	class SpriteRenderQueue : public RenderQueue
	{
	public:
		virtual void Render() override;

		static bool Compare(GameObject* sprite1, GameObject* sprite2);
	};

	class NormalRenderQueue : public RenderQueue
	{
	public:
		NormalRenderQueue();
		virtual ~NormalRenderQueue();
	};

	class GlobalRenderQueue : public RenderQueue
	{
	public:
		GlobalRenderQueue();
		virtual ~GlobalRenderQueue();

		virtual void Render() override;
	};
}

#endif