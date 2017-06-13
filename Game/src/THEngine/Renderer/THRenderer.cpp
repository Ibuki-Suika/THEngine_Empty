#ifndef TH_RENDERER_H
#define TH_RENDERER_H

#include "THRenderer.h"
#include <Platform\THApplication.h>
#include <Core\THGameObject.h>

namespace THEngine
{
	void Renderer::SetupRenderState(GameObject* obj)
	{
		auto app = Application::GetInstance();

		app->SetBlendMode(obj->GetBlendMode());
	}

	void Renderer::SetupWorldTransform(GameObject* object)
	{
		Matrix world;
		object->GetWorldMatrix(&world);
		Application::GetInstance()->SetWorldMatrix(world);
	}
}

#endif