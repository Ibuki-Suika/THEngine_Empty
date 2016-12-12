#include "THRenderObject.h"
#include "../Tween/THConcreteTween.h"

namespace THEngine
{
	RenderObject::RenderObject()
	{
		tweenManager = new TweenManager();
		tweenManager->Retain();

		D3DXQuaternionIdentity(&rotation3D);
	}

	RenderObject::RenderObject(const RenderObject& object) : GameObject(object)
	{
		position = object.position;
		color = object.color;
		alpha = object.alpha;
		scale = object.scale;
		rotation3D = object.rotation3D;

		tweenManager = new TweenManager(*object.tweenManager);
		tweenManager->Retain();
	}

	RenderObject::~RenderObject()
	{
		TH_SAFE_RELEASE(tweenManager);
	}

	Object* RenderObject::Clone()
	{
		return new RenderObject(*this);
	}

	void RenderObject::Update()
	{
		GameObject::Update();
		tweenManager->Update();
	}

	void RenderObject::AddTween(Tween* tween)
	{
		tween->Bind(this);
		tweenManager->AddTween(tween);
	}

	void RenderObject::SetRotationByAxis(const Vector3f& axis, float rotation)
	{
		D3DXQuaternionRotationAxis(&rotation3D, &D3DXVECTOR3(axis.x, axis.y, axis.z), ToRad(rotation));
	}
}