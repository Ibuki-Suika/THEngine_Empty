#include "THLight.h"

namespace THEngine
{
	Light::Light()
	{

	}

	Light::Light(const Vector3f& color) : color(color)
	{

	}

	Light::~Light()
	{

	}

	/////////////////////////////////////////////////
	DirectionalLight::DirectionalLight()
	{
		type = DIRECTIONAL;
	}

	DirectionalLight::DirectionalLight(const Vector3f& color, const Vector3f& direction) :
		Light(color),
		direction(direction)
	{
		type = DIRECTIONAL;
	}

	DirectionalLight::~DirectionalLight()
	{

	}
}