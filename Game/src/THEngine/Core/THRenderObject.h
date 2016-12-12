#ifndef THRENDEROBJECT_H
#define THRENDEROBJECT_H

#include "THGameObject.h"
#include "../Tween/THTweenManager.h"

namespace THEngine
{
	class RenderObject : public GameObject
	{
	protected:
		Vector3f position = Vector3f(0.0f, 0.0f, 0.0f);
		Vector3f color = Vector3f(1.0f, 1.0f, 1.0f);
		float alpha = 1.0f;

		Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f);

		D3DXQUATERNION rotation3D;

		TweenManager* tweenManager = nullptr;

	public:
		RenderObject();
		RenderObject(const RenderObject& object);
		virtual ~RenderObject();

		virtual Object* Clone() override;

		virtual void Update() override;

		void AddTween(Tween* tween);

		inline void KillTween(Tween* tween)
		{
			tweenManager->KillTween(tween);
		}

		inline void SetPosition(const Vector3f& position)
		{
			this->position = position;
		}

		inline Vector3f GetPosition()
		{
			return position;
		}

		inline void SetColor(const Vector3f& color)
		{
			this->color = color;
		}

		inline Vector3f GetColor()
		{
			return color;
		}

		inline float GetAlpha() { return alpha; }
		inline void SetAlpha(float alpha) { this->alpha = alpha; }

		inline void SetScale(Vector3f scale) { this->scale = scale; }
		inline Vector3f GetScale() { return scale; }

		void SetRotationByAxis(const Vector3f& axis, float rotation);

		friend class MoveTo;
		friend class MoveBy;
		friend class FadeTo;
		friend class FadeOut;
		friend class ColorTo;
		friend class ScaleTo;
	};
}

#endif