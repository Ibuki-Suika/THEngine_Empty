#ifndef THGAMEOBJECT_H
#define THGAMEOBJECT_H

#include "../Common/THCommon.h"
#include "../Math/MathUtil.h"

namespace THEngine
{
	class Layer;
	class RenderQueue;

	class GameObject : public EngineObject
	{
	protected:
		LinkedList<GameObject*> children;
		bool needRemove;

	protected:
		virtual void SendToRenderQueue();

	private:
		void Visit();

	public:
		GameObject();
		virtual ~GameObject();
		virtual void Update() override;
		virtual void Draw() override;
		
		virtual void OnDestroy();

		void AddChild(GameObject* obj);

		void RemoveChild(GameObject* child);

		inline void MarkDestroy() { needRemove = true; }

		inline bool NeedRemove() { return needRemove; }

		friend class Layer;
		friend class RenderQueue;
	};
}

#endif