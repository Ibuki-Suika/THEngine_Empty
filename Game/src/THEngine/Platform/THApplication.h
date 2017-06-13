#ifndef THAPPLICATION_H
#define THAPPLICATION_H

#include <Common\THCommon.h>
#include <Core\THRenderState.h>

namespace THEngine
{
	class Input;
	class RenderTexture;
	class Surface;
	struct Config;

	class Application : public Object
	{
	private:
		const Config* config = nullptr; //
		int bigIcon, smallIcon;
		bool needQuit;
		int returnCode;

		HINSTANCE hInstance;
		HWND hWnd;

		//DirectX device
		IDirect3D9* d3d;
		IDirect3DDevice9* device;
		D3DPRESENT_PARAMETERS d3dpp;

		RenderState renderState;

		static Application* instance;

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		bool RegisterGameClass();
		bool CreateGameWindow();
		bool InitDeviceContext();
		void InitRenderState();
		void GetDeviceInfo(D3DDEVTYPE* deviceType, int* vertexProcessingType);
		void GetMultiSampleType(D3DDEVTYPE deviceType, D3DMULTISAMPLE_TYPE* multiSampleType, DWORD* qualityLevel);
		bool CheckDeviceCaps(D3DDEVTYPE deviceType);
		bool CreateDevice();

		void OnKeyDown(int key);

	public:
		Application();
		~Application();

		static Application* GetInstance();

		bool Init(const Config& config, int bigIcon, int smallIcon);
		void DealWithMessage();

		void SetOrtho(float left, float bottom, float width, float height, float znear, float zfar);

		void SetViewport(int left, int top, int width, int height);

		void PrintScreen();

		bool IsDeviceLost();

		bool NeedResetDevice();

		void OnLostDevice();

		void OnResetDevice();

		void ResetDeviceState();

		bool IsMinimized();

		inline IDirect3DDevice9* GetDevice() { return device; }

		inline RenderState* GetRenderState() { return &renderState; }

		inline bool NeedQuit() { return needQuit; }

		inline int GetReturnCode() { return returnCode; }

		inline HWND GetWindowHandle() { return hWnd; }

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

		inline void Quit()
		{
			PostQuitMessage(0);
		}

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

		friend class AssetManager;
		friend class SpriteRenderer;
		friend class Particle3DRenderer;
		friend class MeshRenderer;
		friend class Input;
		friend class Audio;
	};
}

#endif