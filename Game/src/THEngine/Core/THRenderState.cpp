#include "THRenderState.h"
#include "THEnvironment.h"
#include <Asset\THShader.h>
#include <Asset\THRenderTexture.h>
#include <Platform\THSurface.h>
#include <Core\THCamera.h>

namespace THEngine
{
	RenderState::RenderState()
	{
	}

	RenderState::~RenderState()
	{
		TH_SAFE_RELEASE(this->shader);
		TH_SAFE_RELEASE(this->renderTarget);
		TH_SAFE_RELEASE(this->depthBuffer);
		TH_SAFE_RELEASE(this->colorBuffer);
		TH_SAFE_RELEASE(this->camera);
	}

	const Vector4f& RenderState::GetAmbientLight() const
	{
		return environment->ambientLight;
	}

	LinkedList<Light*>* RenderState::GetLights() const
	{
		return &environment->lights;
	}

	const Fog& RenderState::GetFog() const
	{
		return this->environment->fog;
	}

	bool RenderState::IsFogEnabled() const
	{
		return this->environment->fogEnable;
	}

	bool RenderState::IsLightingEnabled() const
	{
		return this->environment->lightingEnable;
	}

	void RenderState::Clear()
	{
	}
}