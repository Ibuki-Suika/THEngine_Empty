#ifndef THLAYER_H
#define THLAYER_H

#include "../Common/THCommon.h"
#include "THGameObject.h"
#include "THCamera.h"
#include "../Platform/THApplication.h"
#include "3D\THLight.h"

namespace THEngine
{
	class Layer : public EngineObject
	{
	protected:
		int width, height;
		int left, top;
		int order;          
		GameObject rootNode;
		Camera* camera = nullptr;

		bool activated = false;

		bool fogEnable = false;
		Fog fog;

		LinkedList<Light*> lights;

		bool lightingEnable = false;

	protected:
		void SetupRenderState();

	public:
		Layer();
		Layer(int left, int top, int wdith, int height);
		virtual ~Layer();
		virtual void Update() override;
		virtual void Draw() override;

		virtual void OnActivate();
		virtual void OnDestroy();

		void AddChild(GameObject* obj);

		inline void AddLight(Light* light) { lights.Add(light); }

		inline void RemoveLight(Light* light) { lights.Remove(light); }

		inline Camera* GetCamera() { return camera; }

		void SetCamera(Camera* camera);

		inline void SetOrder(int order) { this->order = order; }

		inline void EnableFog(bool fogEnable) { this->fogEnable = fogEnable; }

		inline void EnableLighting(bool lightingEnable) { this->lightingEnable = lightingEnable; }
		
		inline void SetFog(Fog fog) { this->fog = fog; }

		void DestroyObjectImmediately(GameObject* obj);

		friend class Scene;
	};
}

#endif