#ifndef THRENDERSTATE_H
#define THRENDERSTATE_H

#include "../Common/THCommon.h"
#include "../Math/Matrix.h"
#include "3D\THLight.h"

namespace THEngine
{
	struct Fog
	{
		Vector4f fogColor;
		float fogStart;
		float fogEnd;
	};

	class RenderState : public Object
	{
	public:
		bool fogEnable = false;
		Fog fog;

		bool lightingEnable = false;

		D3DXMATRIX world, projection, view;

		ArrayList<DirectionalLight*> directionalLights;

	public:
		RenderState();
		virtual ~RenderState();

		void Clear();
	};
}

#endif