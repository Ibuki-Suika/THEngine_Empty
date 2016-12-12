#ifndef THSPRITE_H
#define THSPRITE_H

#include "THRenderObject.h"
#include "../Asset/THTexture.h"
#include "../Renderer/THRenderer.h"

namespace THEngine
{
	class Renderer;

	class Sprite : public RenderObject
	{
	public:

	protected:
		Texture* texture;
		Rect texRect;
		Renderer* renderer;

		
		Vector2f pivot;

		float rotation;

		float speed;
		float angle;
		float acSpeed;
		float acAngle;

		float ax, ay, vx, vy;

		bool flipX;
		bool flipY;

	private:
		virtual void SendToRenderQueue() override final;

	public:
		Sprite();
		virtual ~Sprite();

		virtual void Update() override;
		virtual void Draw() override;

		inline void SetTexture(Texture* texture) { 
			this->texture = texture; 
		}
		inline void SetTexRect(Rect rect) { this->texRect = rect; }
		inline Rect GetTexRect() { return texRect; }
		inline Renderer* GetRenderer() { return renderer; }
		inline void SetPivot(Vector2f pivot) { this->pivot = pivot; }
		
		inline float GetRotation() { return rotation; }
		inline void SetRotation(float rotation) { this->rotation = rotation; }

		inline void SetSpeed(float speed) { this->speed = speed; }
		inline float GetSpeed() { return speed; }

		inline void SetAngle(float angle) { this->angle = angle; }
		inline float GetAngle() { return angle; }

		inline void SetAcSpeed(float acSpeed) { this->acSpeed = acSpeed; }
		inline float GetAcSpeed() { return acSpeed; }

		friend class SpriteRenderer;

		inline void SetFlipX(bool flipX) { this->flipX = flipX; }
		inline void SetFlipY(bool flipY) { this->flipY = flipY; }
		
		inline void SetScaleX(float x) { this->scale.x = x; }
		inline void SetScaleY(float y) { this->scale.y = y; }
		inline void SetScale(float x, float y) { SetScaleX(x); SetScaleY(y); }
		inline float GetScaleX() { return scale.x; }
		inline float GetScaleY() { return scale.y; }

		friend class Rotate2D;
	};
}

#endif