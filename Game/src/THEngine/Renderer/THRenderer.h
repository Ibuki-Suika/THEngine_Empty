#ifndef THRENDERER_H
#define THRENDERER_H

#include <Common\THCommon.h>

namespace THEngine
{
	class GameObject;

	class Renderer : public Object
	{
	protected:
		void SetupRenderState(GameObject* object);
		void SetupWorldTransform(GameObject* object);

	public:
		virtual void Render(GameObject* obj) = 0;
	};
}

#endif