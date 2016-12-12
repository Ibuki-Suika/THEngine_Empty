#ifndef THAPPLICATION_H
#define THAPPLICATION_H

#include "../Common/THCommon.h"
#include "../Core/THRenderState.h"

namespace THEngine
{
	class Input;

	class Application : public Object
	{
	private:
		int bigIcon;
		int smallIcon;
		String title;
		int width, height;
		bool fullScreen;
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
		void GetDeviceInfo(D3DDEVTYPE* deviceType, int* vertexProcessingType);
		void GetMultiSampleType(D3DDEVTYPE deviceType, D3DMULTISAMPLE_TYPE* multiSampleType, DWORD* qualityLevel);
		bool CheckDeviceCaps(D3DDEVTYPE deviceType);
		bool CreateDevice();

		void OnKeyDown(int key);

	public:
		Application();
		~Application();

		static Application* GetInstance();

		bool Init(int width, int height, bool fullScreen, String title, int bigIcon, int smallIcon);
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

		inline void ClearBuffer()
		{
			device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
				D3DCOLOR_ARGB(255, 0, 0, 0), 1, 0);
		}

		inline void ClearDepthBuffer()
		{
			device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1, 0);
		}

		inline bool SwapBuffers()
		{
			return (!FAILED(device->Present(NULL, NULL, NULL, NULL)));
		}

		inline void BeginRender()
		{
			device->BeginScene();
		}

		inline void EndRender()
		{
			device->EndScene();
		}

		inline void Quit()
		{
			PostQuitMessage(0);
		}

		inline void SetWorldTransform(D3DXMATRIX* world)
		{
			renderState.world = *world; 
		}

		inline void SetProjectionTransform(D3DXMATRIX* projection) 
		{ 
			renderState.projection = *projection; 
		}

		inline void SetViewTransform(D3DXMATRIX* view) { 
			renderState.view = *view; 
		}

		friend class AssetManager;
		friend class SpriteRenderer;
		friend class Particle3DRenderer;
		friend class MeshRenderer;
		friend class Input;
		friend class Audio;
	};
}

#endif