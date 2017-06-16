#ifndef THSPRITE_H
#define THSPRITE_H

#include "THGameObject.h"

namespace THEngine
{
	class Renderer;
	class Texture;

	class Sprite : public GameObject
	{
	protected:
		Texture* texture;
		Rect texRect;

		Vector2f pivot;

		float rotation;

		float speed;
		float angle;
		float acSpeed;
		float acAngle;

		float ax, ay, vx, vy;

		bool flipX;
		bool flipY;

		bool pixelAlign = false;

	private:
		virtual void SendToRenderQueue() override final;

	public:
		Sprite();
		virtual ~Sprite();

		virtual void Update() override;
		virtual void Draw() override;

		void SetTexture(Texture* texture);

		inline Texture* GetTexture() const
		{
			return this->texture;
		}

		inline void SetTexRect(const Rect& rect) { this->texRect = rect; }
		inline const Rect& GetTexRect() const { return texRect; }
		inline void SetPivot(Vector2f pivot) { this->pivot = pivot; }

		inline float GetRotation() const { return rotation; }
		inline void SetRotation(float rotation) { this->rotation = rotation; }

		inline void SetSpeed(float speed) { this->speed = speed; }
		inline float GetSpeed() const { return speed; }

		inline void SetAngle(float angle) { this->angle = angle; }
		inline float GetAngle() const { return angle; }

		inline void SetAcSpeed(float acSpeed) { this->acSpeed = acSpeed; }
		inline float GetAcSpeed() const { return acSpeed; }

		inline void SetAcAngle(float acAngle) { this->acAngle = acAngle; }
		inline float GetAcAngle() const { return acAngle; }

		friend class SpriteRenderer;

		inline void SetFlipX(bool flipX) { this->flipX = flipX; }
		inline void SetFlipY(bool flipY) { this->flipY = flipY; }

		inline void SetScaleX(float x) { this->scale.x = x; }
		inline void SetScaleY(float y) { this->scale.y = y; }
		inline void SetScale(float x, float y) { SetScaleX(x); SetScaleY(y); }
		inline float GetScaleX() const { return scale.x; }
		inline float GetScaleY() const { return scale.y; }

		inline void SetPixelAlign(bool pixelAlign) { this->pixelAlign = pixelAlign; }

		friend class Rotate2D;
		friend class SpeedTo;
	};
}

#endif