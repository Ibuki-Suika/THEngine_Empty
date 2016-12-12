#ifndef THRENDERER_H
#define THRENDERER_H

#include "../Common/THCommon.h"
#include "THRenderQueue.h"
#include "../Asset/THShader.h"


namespace THEngine
{
	class RenderObject;

	class Renderer : public Object
	{
	protected:
		IDirect3DDevice9* device;

	public:
		virtual void Render(RenderObject* obj) = 0;
	};
}

#endif