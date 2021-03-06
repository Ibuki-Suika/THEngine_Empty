#ifndef TH_RENDERER_H
#define TH_RENDERER_H

#include "THRenderer.h"
#include <Platform\THDevice.h>
#include <Core\THGameObject.h>

namespace THEngine
{
	void Renderer::SetupRenderState(Ptr<GameObject> obj)
	{
		auto device = Device::GetInstance();

		device->SetBlendMode(obj->GetBlendMode());
	}

	void Renderer::SetupWorldTransform(Ptr<GameObject> object)
	{
		Matrix world;
		object->GetWorldMatrix(&world);
		Device::GetInstance()->SetWorldMatrix(world);
	}
}

#endif