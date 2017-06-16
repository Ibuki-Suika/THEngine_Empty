#ifndef TH_DEVICE_H
#define TH_DEVICE_H

#include <Common\THCommon.h>
#include <Core\THRenderState.h>

namespace THEngine
{
	struct Config;

	class Device : public Object
	{
	private:
		const Config* config = nullptr;

		//DirectX device
		IDirect3D9* d3d;
		IDirect3DDevice9* device;
		D3DPRESENT_PARAMETERS d3dpp;

		RenderState renderState;

		static Device* instance;

	private:
		void GetDeviceInfo(D3DDEVTYPE* deviceType, int* vertexProcessingType);
		void GetMultiSampleType(D3DDEVTYPE deviceType, D3DMULTISAMPLE_TYPE* multiSampleType, DWORD* qualityLevel);
		bool CheckDeviceCaps(D3DDEVTYPE deviceType);
		bool CreateDevice(HWND hWnd);
		void InitRenderState();

	public:
		Device();
		virtual ~Device();

		bool Init(const Config* config);

		static Device* GetInstance();

		void SetOrtho(float left, float bottom, float width, float height, float znear, float zfar);
		void SetViewport(int left, int top, int width, int height);
		void ResetDeviceState();

		bool IsDeviceLost();
		bool NeedResetDevice();
		void OnLostDevice();
		void OnResetDevice();

		inline IDirect3DDevice9* GetD3DDevice() { return device; }

		inline RenderState* GetRenderState() { return &renderState; }

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

		void SetRenderTarget(RenderTexture* texture);

		void SetDepthBuffer(Surface* depthBuffer);
		inline Surface* GetDepthBuffer() const { return this->renderState.GetDepthBuffer(); }

		inline Surface* GetColorBuffer() const { return this->renderState.GetColorBuffer(); }

		Surface* CreateDepthBuffer(int width, int height);
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
			renderState.world = world;
		}

		inline void SetProjectionMatrix(const Matrix& projection)
		{
			renderState.projection = projection;
		}

		inline void SetViewMatrix(const Matrix& view)
		{
			renderState.view = view;
		}

		void EnableDepthTest(bool value);
	};
}

#endif
