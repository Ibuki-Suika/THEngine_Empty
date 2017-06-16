#ifndef THAPPLICATION_H
#define THAPPLICATION_H

#include <Common\THCommon.h>

namespace THEngine
{
	class Input;
	class RenderTexture;
	class Surface;
	struct Config;
	class Device;

	class Application : public Object
	{
	private:
		const Config* config = nullptr;
		int bigIcon, smallIcon;
		bool needQuit;
		int returnCode;

		HINSTANCE hInstance;
		HWND hWnd;

		Device* device = nullptr;

		static Application* instance;

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		bool RegisterGameClass();
		bool CreateGameWindow();
		bool InitDeviceContext();

		void OnKeyDown(int key);
	public:
		Application();
		~Application();

		static Application* GetInstance();

		bool Init(const Config& config, int bigIcon, int smallIcon);
		void DealWithMessage();

		bool IsMinimized();
		void PrintScreen();

		inline HWND GetHwnd() const { return hWnd; }
		inline Device* GetDevice() { return device; }

		inline bool NeedQuit() { return needQuit; }

		inline int GetReturnCode() { return returnCode; }

		inline HWND GetWindowHandle() { return hWnd; }

		inline void Quit()
		{
			PostQuitMessage(0);
		}

		friend class SpriteRenderer;
		friend class Particle3DRenderer;
		friend class MeshRenderer;
		friend class Input;
		friend class Audio;
	};
}

#endif