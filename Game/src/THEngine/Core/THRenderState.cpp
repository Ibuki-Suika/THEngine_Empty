#include "THRenderState.h"

namespace THEngine
{
	RenderState::RenderState()
	{
		D3DXMatrixIdentity(&world);
		D3DXMatrixIdentity(&projection);
		D3DXMatrixIdentity(&view);
	}

	RenderState::~RenderState()
	{

	}

	void RenderState::Clear()
	{
		directionalLights.Clear();
	}
}