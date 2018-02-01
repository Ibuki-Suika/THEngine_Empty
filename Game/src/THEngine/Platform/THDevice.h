#ifndef TH_DEVICE_H
#define TH_DEVICE_H

#include <Common\THCommon.h>
#include <Core\THRenderState.h>

namespace THEngine
{
	struct Config;

	class Device : public Object, public Singleton<Device>
	{
		friend class Singleton<Device>;

	private:
		const Config* config = nullptr;

		//DirectX device
		IDirect3D9* d3d = nullptr;
		IDirect3DDevice9* device = nullptr;
		D3DPRESENT_PARAMETERS d3dpp;

		Ptr<RenderState> renderState = Ptr<RenderState>::New();
	private:
		Device() = default;

		void GetDeviceInfo(D3DDEVTYPE* deviceType, int* vertexProcessingType);
		void GetMultiSampleType(D3DDEVTYPE deviceType, D3DMULTISAMPLE_TYPE* multiSampleType, DWORD* qualityLevel);
		bool CheckDeviceCaps(D3DDEVTYPE deviceType);
		bool CreateDevice(HWND hWnd);
		void InitRenderState();

	public:
		virtual ~Device();

		bool Init(const Config* config);

		void SetOrtho(float left, float bottom, float width, float height, float znear, float zfar);
		void SetViewport(int left, int top, int width, int height);
		void ResetDeviceState();

		bool IsDeviceLost();
		bool NeedResetDevice();
		void OnLostDevice();
		void OnResetDevice();

		inline IDirect3DDevice9* GetD3DDevice() { return device; }

		inline Ptr<RenderState> GetRenderState() { return renderState; }

		inline void ClearBuffer()
		{
			device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
				D3DCOLOR_ARGB(255, 0, 0, 0), 1, 0);
		}

		inline void ClearColorBuffer(const Vector4f& color)
		{
			device->Clear(0, NULL, D3DCLEAR_TARGET,
				D3DCOLOR_ARGB((int)(255 * color.w + 0.5), ((int)(255 * color.x + 0.5)), ((int)(255 * color.y + 0.5)),
				((int)(255 * color.z + 0.5))), 1, 0);
		}

		inline void ClearDepthBuffer()
		{
			device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1, 0);
		}

		void SetRenderTarget(Ptr<RenderTexture> texture);

		void SetDepthBuffer(Ptr<Surface> depthBuffer);
		inline Ptr<Surface> GetDepthBuffer() const { return this->renderState->GetDepthBuffer(); }

		inline Ptr<Surface> GetColorBuffer() const { return this->renderState->GetColorBuffer(); }

		Ptr<Surface> CreateDepthBuffer(int width, int height);
		void SetBlendMode(BlendMode blendMode);

		inline bool SwapBuffers()
		{
			return (!FAILED(device->Present(NULL, NULL, NULL, NULL)));
		}

		inline void BeginRender()
		{
			device->BeginScene();
		}

		void EndRender();

		inline void SetWorldMatrix(const Matrix& world)
		{
			renderState->world = world;
		}

		inline void SetProjectionMatrix(const Matrix& projection)
		{
			renderState->projection = projection;
		}

		inline void SetViewMatrix(const Matrix& view)
		{
			renderState->view = view;
		}

		void EnableDepthTest(bool value);
	};
}

#endif
