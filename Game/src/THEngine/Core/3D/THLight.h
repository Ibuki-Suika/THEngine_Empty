#ifndef THLIGHT_H
#define THLIGHT_H

#include "../../Common/THCommon.h"
#include "../../Math/Matrix.h"

namespace THEngine
{
	class Light : public Object
	{
	public:
		enum Type
		{
			DIRECTIONAL,
			POINT,
			SPOTLIGHT
		};

		Type type;

	protected:
		Vector3f ambient = Vector3f(0.0f, 0.0f, 0.0f);
		Vector3f color = Vector3f(1.0f, 1.0f, 1.0f);

	public:
		Light();
		virtual ~Light();

		inline Type GetType() const { return type; }
	};

	class DirectionalLight : public Light
	{
	private:
		Vector3f direction;

	public:
		DirectionalLight();
		virtual ~DirectionalLight();
	};
}

#endif