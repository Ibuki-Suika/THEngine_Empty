#include "THApplication.h"
#include "THDevice.h"
#include "THSurface.h"
#include <UI\THEventSystem.h>
#include <Core\THConfig.h>

namespace THEngine
{
	Application* Application::instance = nullptr;

	Application::Application()
	{
		ASSERT(instance == nullptr);
		needQuit = false;
		instance = this;
	}

	Application::~Application()
	{
		UnregisterClass(TEXT("THGameEngine"), hInstance);

		TH_SAFE_RELEASE(this->device);
	}

	Application* Application::GetInstance()
	{
		return instance;
	}

	bool Application::Init(const Config& config, int bigIcon, int smallIcon)
	{
		auto exceptionManager = ExceptionManager::GetInstance();

		bool status;

		this->config = &config;
		this->bigIcon = bigIcon;
		this->smallIcon = smallIcon;

		hInstance = (HINSTANCE)GetModuleHandle(NULL);

		status = RegisterGameClass();
		if (status == false)
		{
			exceptionManager->PushException(new Exception("注册窗口类失败。"));
			return status;
		}

		status = CreateGameWindow();
		if (status == false)
		{
			exceptionManager->PushException(new Exception("创建窗口失败。"));
			return status;
		}

		if (config.fullScreen)
		{
			ShowCursor(FALSE);
		}

		status = InitDeviceContext();
		if (status == false)
		{
			auto exception = exceptionManager->GetException();
			exceptionManager->PushException(new Exception((String)"初始化Direct3D失败。原因是：\n" + exception->GetInfo()));
			return status;
		}

		return true;
	}

	bool Application::RegisterGameClass()
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(bigIcon));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = TEXT("THGameEngine");
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(smallIcon));

		if (RegisterClassEx(&wcex) == 0)
		{
			return false;
		}
		return true;
	}

	bool Application::CreateGameWindow()
	{
		UINT windowStyle = WS_OVERLAPPEDWINDOW;
		windowStyle &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);            //固定窗口大小

		hWnd = CreateWindow(TEXT("THGameEngine"), config->title.GetBuffer(), windowStyle,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

		if (!hWnd)
		{
			return false;
		}

		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);

		//移动窗口至屏幕中央并设置客户区大小
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);

		int screen_width = GetSystemMetrics(SM_CXSCREEN);
		int screen_height = GetSystemMetrics(SM_CYSCREEN);

		MoveWindow(hWnd, (screen_width - config->width) / 2, (screen_height - config->height) / 2,
			config->width + windowRect.right - windowRect.left - clientRect.right - clientRect.left,
			config->height + windowRect.bottom - windowRect.top - clientRect.bottom - clientRect.top, TRUE);

		return true;
	}

	bool Application::InitDeviceContext()
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		THLog((String)"游戏开始于" + time.wYear + "年" + time.wMonth + "月" + time.wDay + "日"
			+ time.wHour + ":" + time.wMinute + ":" + time.wSecond + "。");

		this->device = new Device();
		if (device->Init(config) == false)
		{
			return false;
		}
		this->device->Retain();

		return true;
	}

	void Application::DealWithMessage()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				needQuit = true;
				returnCode = (int)msg.wParam;
			}
		}
	}

	void Application::OnKeyDown(int key)
	{
		if (key == VK_HOME)
		{
			PrintScreen();
		}
		EventSystem::GetInstance()->OnKeyDown(key);
	}

	void Application::PrintScreen()
	{
		Surface* surface = this->device->GetColorBuffer();

		String dir = "snapshot";
		String path;

		WIN32_FIND_DATAW wfd;
		HANDLE handle = FindFirstFile(dir.GetBuffer(), &wfd);
		if (handle == INVALID_HANDLE_VALUE || ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
		{
			//snapshot目录不存在,创建之
			CreateDirectory(dir.GetBuffer(), NULL);
		}
		int i = 1;
		while (true)
		{
			path = dir + "\\" + i + ".jpg";
			handle = FindFirstFile(path.GetBuffer(), &wfd);
			if (handle == INVALID_HANDLE_VALUE)
			{
				break;
			}
			i++;
		}

		surface->SaveToFile(path);
	}

	bool Application::IsMinimized()
	{
		if (IsIconic(hWnd))
		{
			return true;
		}
		return false;
	}

	LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO:  在此添加任意绘图代码...
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			Application::GetInstance()->OnKeyDown(wParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}