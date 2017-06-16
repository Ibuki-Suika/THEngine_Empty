#ifndef THLIGHT_H
#define THLIGHT_H

#include <Common\THCommon.h>
#include <Math\THMatrix.h>

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
		Vector3f color = Vector3f(1.0f, 1.0f, 1.0f);

	public:
		Light();
		Light(const Vector3f& color);
		virtual ~Light();

		inline Type GetType() const { return type; }

		inline const Vector3f& GetColor() const { return color; }
	};

	class DirectionalLight : public Light
	{
	private:
		Vector3f direction;

	public:
		DirectionalLight();
		DirectionalLight(const Vector3f& color, const Vector3f& direction);
		virtual ~DirectionalLight();

		inline const Vector3f& GetDirection() const { return direction; }
		inline void SetDirection(const Vector3f& direction) { this->direction = direction; }
	};
}

#endif