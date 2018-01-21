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

		LinkedList<Ptr<Camera>> cameraList;

		Environment environment;

		Ptr<CubeMap> skyBox = nullptr;

	protected:
		void SetupRenderState();

	public:
		Layer();
		Layer(int left, int top, int wdith, int height);
		virtual ~Layer();
		virtual void Update() override;
		virtual void Draw() override;
		virtual void OnLoad(Ptr<AsyncInfo> info) override;

		virtual void OnDestroy();

		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }
		inline int GetLeft() const { return left; }
		inline int GetTop() const { return top; }

		void AddChild(Ptr<GameObject> obj);

		void Clear();

		inline void AddLight(Ptr<Light> light) { this->environment.lights.Add(light); }

		inline void RemoveLight(Ptr<Light> light) { this->environment.lights.Remove(light); }

		inline LinkedList<Ptr<Camera>>* GetCameraList() { return &cameraList; }

		Ptr<Camera> GetCameraByName(const String& name);
		Ptr<Camera> GetCameraByIndex(int index);
		Ptr<Camera> GetFirstCamera();

		void AddCamera(Ptr<Camera> camera);

		void SetCameraByName(Ptr<Camera> camera, const String& name);
		void SetCameraByIndex(Ptr<Camera> camera, int index);
		void SetFirstCamera(Ptr<Camera> camera);

		inline void SetOrder(int order) { this->order = order; }

		inline void EnableFog(bool fogEnable) { this->environment.fogEnable = fogEnable; }

		inline void EnableLighting(bool lightingEnable) { this->environment.lightingEnable = lightingEnable; }

		inline void SetAmbientLight(const Vector4f ambient) { this->environment.ambientLight = ambient; }

		inline void SetFog(Fog fog) { this->environment.fog = fog; }

		void SetSkyBox(Ptr<CubeMap> skyBox);

		inline Ptr<CubeMap> GetSkyBox() const { return skyBox; }

		void DestroyObjectImmediately(GameObject* obj);

		friend class Scene;
	};
}

#endif