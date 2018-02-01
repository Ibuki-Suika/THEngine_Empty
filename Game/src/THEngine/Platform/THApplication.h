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

	class Application : public Object, public Singleton<Application>
	{
		friend class Singleton<Application>;

	private:
		const Config* config = nullptr;
		int bigIcon, smallIcon;
		bool needQuit = false;
		int returnCode;

		HINSTANCE hInstance;
		HWND hWnd;

		Ptr<Device> device;
	private:
		Application() = default;

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		bool RegisterGameClass();
		bool CreateGameWindow();
		bool InitDeviceContext();

		void OnKeyDown(int key);
	public:
		~Application();

		bool Init(const Config& config, int bigIcon, int smallIcon);
		void DealWithMessage();

		bool IsMinimized();
		void PrintScreen();

		inline HWND GetHwnd() const { return hWnd; }
		inline Ptr<Device> GetDevice() { return device; }

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