#ifndef THLAYER_H
#define THLAYER_H

#include <Common\THCommon.h>
#include "THGameObject.h"
#include "THCamera.h"
#include "THEnvironment.h"

namespace THEngine
{
	class CubeMap;

	class Layer : public EngineObject
	{
	protected:
		int width, height;
		int left, top;
		int order;
		GameObject rootNode;

		LinkedList<Camera*> cameraList;

		Environment environment;

		CubeMap* skyBox = nullptr;

	protected:
		void SetupRenderState();

	public:
		Layer();
		Layer(int left, int top, int wdith, int height);
		virtual ~Layer();
		virtual void Update() override;
		virtual void Draw() override;
		virtual void OnLoad(AsyncInfo* info) override;

		virtual void OnDestroy();

		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }
		inline int GetLeft() const { return left; }
		inline int GetTop() const { return top; }

		void AddChild(GameObject* obj);

		void Clear();

		inline void AddLight(Light* light) { this->environment.lights.Add(light); }

		inline void RemoveLight(Light* light) { this->environment.lights.Remove(light); }

		inline LinkedList<Camera*>* GetCameraList() { return &cameraList; }

		Camera* GetCameraByName(const String& name);
		Camera* GetCameraByIndex(int index);
		Camera* GetFirstCamera();

		void AddCamera(Camera* camera);

		void SetCameraByName(Camera* camera, const String& name);
		void SetCameraByIndex(Camera* camera, int index);
		void SetFirstCamera(Camera* camera);

		inline void SetOrder(int order) { this->order = order; }

		inline void EnableFog(bool fogEnable) { this->environment.fogEnable = fogEnable; }

		inline void EnableLighting(bool lightingEnable) { this->environment.lightingEnable = lightingEnable; }

		inline void SetAmbientLight(const Vector4f ambient) { this->environment.ambientLight = ambient; }

		inline void SetFog(Fog fog) { this->environment.fog = fog; }

		void SetSkyBox(CubeMap* skyBox);

		inline CubeMap* GetSkyBox() const { return skyBox; }

		void DestroyObjectImmediately(GameObject* obj);

		friend class Scene;
	};
}

#endif