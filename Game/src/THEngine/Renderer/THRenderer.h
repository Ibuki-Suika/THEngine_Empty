#ifndef THRENDERER_H
#define THRENDERER_H

#include <Common\THCommon.h>

namespace THEngine
{
	class GameObject;

	class Renderer : public Object
	{
	protected:
		void SetupRenderState(Ptr<GameObject> object);
		void SetupWorldTransform(Ptr<GameObject> object);

	public:
		virtual void Render(Ptr<GameObject> obj) = 0;
	};
}

#endif