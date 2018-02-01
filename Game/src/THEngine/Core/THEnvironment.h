#ifndef TH_ENVIRONMENT_H
#define TH_ENVIRONMENT_H

#include <Common/THCommon.h>
#include <Math/THVector.h>
#include "3D/THLight.h"

namespace THEngine
{
	struct Fog
	{
		Vector4f fogColor;
		float fogStart;
		float fogEnd;
	};

	class Environment
	{
	private:
		bool fogEnable = false;
		Fog fog;

		Vector4f ambientLight = Vector4f(0, 0, 0, 1);
		LinkedList<Ptr<Light>> lights;

		bool lightingEnable = false;

	public:
		void Clear();

		friend class Layer;
		friend class RenderState;
	};
}

#endif
