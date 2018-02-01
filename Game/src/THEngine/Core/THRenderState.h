#ifndef TH_RENDER_STATE_H
#define TH_RENDER_STATE_H

#include <Common\THCommon.h>
#include <Math\THMath.h>
#include "3D\THLight.h"

namespace THEngine
{
	class RenderTexture;
	class Surface;
	class Camera;
	class Environment;
	struct Fog;

	struct Viewport
	{
		int x;
		int y;
		int width;
		int height;
		int minZ;
		int maxZ;
	};

	enum BlendMode
	{
		ALPHA_BLEND = 1,
		ADD = 2
	};

	class RenderState : public Object
	{
	private:
		Environment* environment;

		Matrix world;
		Matrix projection;
		Matrix view;

		Viewport viewport;

		BlendMode blendMode;

		Ptr<Shader> shader;

		Ptr<RenderTexture> renderTarget;
		Ptr<Surface> depthBuffer;
		Ptr<Surface> colorBuffer;;
		Ptr<Camera> camera;

		bool isDepthTestEnabled = true;

	public:
		RenderState();
		virtual ~RenderState();

		bool IsFogEnabled() const;
		bool IsLightingEnabled() const;

		const Fog& GetFog() const;

		inline const Matrix& GetWorldMatrix() { return this->world; }
		inline const Matrix& GetProjectionMatrix() { return this->projection; }
		inline const Matrix& GetViewMatrix() { return this->view; }
		inline const Viewport& GetViewport() const { return this->viewport; }

		const Vector4f& GetAmbientLight() const;
		LinkedList<Ptr<Light>>* GetLights() const;

		inline Ptr<Shader> GetCurrentShader() const { return this->shader; }

		inline Ptr<RenderTexture> GetRenderTarget() const { return this->renderTarget; }

		inline Ptr<Surface> GetDepthBuffer() const { return this->depthBuffer; }
		inline Ptr<Surface> GetColorBuffer() const { return this->colorBuffer; }

		inline Ptr<Camera> GetCamera() const { return this->camera; }

		inline BlendMode GetBlendMode() const { return this->blendMode; }

		inline bool IsDepthTestEnabled() const { return this->isDepthTestEnabled; }

		void Clear();

		friend class Device;
		friend class Layer;
		friend class Shader;
		friend class Camera;
	};
}

#endif