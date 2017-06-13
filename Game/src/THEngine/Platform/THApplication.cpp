#include "THApplication.h"
#include <UI\THEventSystem.h>
#include <Asset\THAssetManager.h>
#include <Asset\THRenderTexture.h>
#include <Core\THConfig.h>
#include "THSurface.h"

namespace THEngine
{
	Application* Application::instance = nullptr;

	Application::Application()
	{
		ASSERT(instance == nullptr);

		d3d = NULL;
		device = NULL;

		needQuit = false;

		instance = this;
	}

	Application::~Application()
	{
		UnregisterClass(TEXT("THGameEngine"), hInstance);

		TH_SAFE_RELEASE(d3d);
		TH_SAFE_RELEASE(device);

		TH_SAFE_DELETE(config);
	}

	Application* Application::GetInstance()
	{
		return instance;
	}

	bool Application::Init(const Config& config, int bigIcon, int smallIcon)
	{
		auto exceptionManager = ExceptionManager::GetInstance();

		bool status;

		this->config = new Config(config);
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

		d3d = Direct3DCreate9(D3D_SDK_VERSION);

		if (CreateDevice() == false)
		{
			return false;
		}

		ResetDeviceState();

		return true;
	}

	void Application::ResetDeviceState()
	{
		device->SetRenderState(D3DRS_ZENABLE, true);
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		device->SetRenderState(D3DRS_LIGHTING, false);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_GAUSSIANQUAD);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_GAUSSIANQUAD);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_GAUSSIANQUAD);

		device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	}

	bool Application::CreateDevice()
	{
		HRESULT hr;

		//设置帧率
		int refresh = D3DPRESENT_RATE_DEFAULT;
		if (config->fullScreen)
		{
			ShowCursor(FALSE);
		}

		D3DDEVTYPE deviceType;
		int vertexProcessingType;

		//获取设备描述
		GetDeviceInfo(&deviceType, &vertexProcessingType);

		//获取多重采样参数
		D3DMULTISAMPLE_TYPE multiSampleType;
		DWORD qualityLevel;

		GetMultiSampleType(deviceType, &multiSampleType, &qualityLevel);

		//检查设备能力
		if (CheckDeviceCaps(deviceType) == false)
		{
			return false;
		}

		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferWidth = config->width;
		d3dpp.BackBufferHeight = config->height;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount = 1;
		d3dpp.MultiSampleType = multiSampleType;
		d3dpp.MultiSampleQuality = qualityLevel;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hWnd;
		d3dpp.Windowed = !config->fullScreen;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.Flags = 0;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		if (config->useVSync)
		{
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}
		else
		{
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		}

		hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, deviceType,
			hWnd, vertexProcessingType | D3DCREATE_MULTITHREADED, &d3dpp, &device);

		InitRenderState();

		if (FAILED(hr))
		{
			return false;
		}

		THLog("Direct3D成功初始化。");
		return true;
	}

	void Application::GetDeviceInfo(D3DDEVTYPE* deviceType, int* vertexProcessingType)
	{
		HRESULT hr;
		hr = d3d->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_UNKNOWN, D3DFMT_UNKNOWN, !config->fullScreen);
		if (FAILED(hr))
		{
			*deviceType = D3DDEVTYPE_HAL;
			THLog("使用硬件渲染模式（HAL）。");
		}
		else
		{
			*deviceType = D3DDEVTYPE_REF;
			THLog("使用软件渲染模式（REF）。");
		}

		D3DCAPS9 d3dcaps;
		ZeroMemory(&d3dcaps, sizeof(d3dcaps));
		d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, *deviceType, &d3dcaps);

		if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		{
			*vertexProcessingType = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			THLog("使用硬件处理顶点。");
		}
		else
		{
			*vertexProcessingType = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			THLog("使用软件处理顶点。");
		}
	}

	bool Application::CheckDeviceCaps(D3DDEVTYPE deviceType)
	{
		auto exceptionManager = ExceptionManager::GetInstance();

		D3DCAPS9 d3dcaps;
		ZeroMemory(&d3dcaps, sizeof(d3dcaps));
		d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &d3dcaps);

		//检查shader版本
		int mainVSVersion = (d3dcaps.VertexShaderVersion & 0x0000ff00) >> 8;
		int subVSVersion = d3dcaps.VertexShaderVersion & 0x000000ff;
		int mainPSVersion = (d3dcaps.PixelShaderVersion & 0x0000ff00) >> 8;
		int subPSVersion = d3dcaps.PixelShaderVersion & 0x000000ff;

		if (mainVSVersion < 2)
		{
			exceptionManager->PushException(new Exception((String)"当前显卡支持的顶点着色器版本过低。\n"
				"当前支持版本：" + mainVSVersion + "." + subVSVersion
				+ "   最低需要版本：2.0"));
			return false;
		}

		if (mainPSVersion < 2)
		{
			exceptionManager->PushException(new Exception((String)"当前显卡支持的像素着色器版本过低。\n"
				"当前支持版本：" + mainPSVersion + "." + subPSVersion
				+ "   最低需要版本：2.0"));
			return false;
		}

		//检查纹理格式支持
		D3DDISPLAYMODE mode;
		d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
		if (FAILED(d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, deviceType, mode.Format, D3DUSAGE_RENDERTARGET,
			D3DRTYPE_TEXTURE, D3DFMT_R32F)))
		{
			exceptionManager->PushException(new Exception("您的显卡不支持32位浮点纹理。"));
			return false;
		}

		THLog("**********************设备信息**********************");
		THLog((String)"顶点着色器版本：" + mainVSVersion + "." + subVSVersion);
		THLog((String)"像素着色器版本：" + mainPSVersion + "." + subPSVersion);
		THLog("****************************************************");

		return true;
	}

	void Application::GetMultiSampleType(D3DDEVTYPE deviceType,
		D3DMULTISAMPLE_TYPE* multiSampleType,
		DWORD* qualityLevel)
	{
		HRESULT hr;

		const D3DMULTISAMPLE_TYPE multiSampleTypes[] = {
			D3DMULTISAMPLE_NONMASKABLE,
			D3DMULTISAMPLE_NONE,
			D3DMULTISAMPLE_2_SAMPLES,
			D3DMULTISAMPLE_3_SAMPLES,
			D3DMULTISAMPLE_4_SAMPLES,
			D3DMULTISAMPLE_5_SAMPLES,
			D3DMULTISAMPLE_6_SAMPLES,
			D3DMULTISAMPLE_7_SAMPLES,
			D3DMULTISAMPLE_8_SAMPLES,
			D3DMULTISAMPLE_9_SAMPLES,
			D3DMULTISAMPLE_10_SAMPLES,
			D3DMULTISAMPLE_11_SAMPLES,
			D3DMULTISAMPLE_12_SAMPLES,
			D3DMULTISAMPLE_13_SAMPLES,
			D3DMULTISAMPLE_14_SAMPLES,
			D3DMULTISAMPLE_15_SAMPLES,
			D3DMULTISAMPLE_16_SAMPLES
		};

		DWORD qualityLevels;
		if (config->useMultiSample)
		{
			*multiSampleType = D3DMULTISAMPLE_16_SAMPLES;
		}
		else
		{
			*multiSampleType = D3DMULTISAMPLE_NONE;
			*qualityLevel = 0;
		}

		for (int i = *multiSampleType; i > 0; i--)
		{
			hr = d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, deviceType, D3DFMT_A8R8G8B8,
				!config->fullScreen, multiSampleTypes[i], &qualityLevels);
			if (!FAILED(hr))
			{
				*multiSampleType = multiSampleTypes[i];
				if (*multiSampleType == D3DMULTISAMPLE_NONE)
				{
					*qualityLevel = 0;
				}
				else
				{
					if (qualityLevels == NULL)
					{
						*qualityLevel = 0;
					}
					else
					{
						*qualityLevel = qualityLevels - 1;
					}
				}
				break;
			}
		}
		if (*multiSampleType == D3DMULTISAMPLE_NONE)
		{
			THLog("不使用多重采样。");
		}
		else
		{
			THLog((String)"使用多重采样。采样数为" + *multiSampleType + "，质量等级" + *qualityLevel + "。");
		}
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

	void Application::EndRender()
	{
		if (renderState.shader)
		{
			renderState.shader->End();
		}
		device->EndScene();
	}

	void Application::InitRenderState()
	{
		this->renderState.depthBuffer = new Surface();
		this->device->GetDepthStencilSurface(&this->renderState.depthBuffer->surface);
		this->renderState.depthBuffer->Retain();
	}

	void Application::SetOrtho(float left, float bottom, float width, float height, float znear, float zfar)
	{
		Matrix projection;
		Matrix::Ortho(&projection, left, left + width, bottom, bottom + height, znear, zfar);
		SetProjectionMatrix(projection);
	}

	void Application::SetViewport(int left, int top, int width, int height)
	{
		renderState.viewport.x = left;
		renderState.viewport.y = top;
		renderState.viewport.width = width;
		renderState.viewport.height = height;
		renderState.viewport.minZ = 0;
		renderState.viewport.maxZ = 1;

		D3DVIEWPORT9 viewport;
		viewport.X = renderState.viewport.x;
		viewport.Y = renderState.viewport.y;
		viewport.Width = renderState.viewport.width;
		viewport.Height = renderState.viewport.height;
		viewport.MinZ = renderState.viewport.minZ;
		viewport.MaxZ = renderState.viewport.maxZ;
		device->SetViewport(&viewport);
	}

	void Application::SetRenderTarget(RenderTexture* texture)
	{
		if (texture)
		{
			IDirect3DSurface9* surface = nullptr;
			texture->texImpl->texture->GetSurfaceLevel(0, &surface);
			device->SetRenderTarget(0, surface);
		}
		else
		{
			IDirect3DSurface9 *surface;
			device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);
			device->SetRenderTarget(0, surface);
		}

		TH_SET(renderState.renderTarget, texture);
	}

	void Application::SetDepthBuffer(Surface* depthBuffer)
	{
		this->device->SetDepthStencilSurface(depthBuffer->surface);
	}

	Surface* Application::CreateDepthBuffer(int width, int height)
	{
		Surface* depthBuffer = new Surface();
		if (FAILED(this->device->CreateDepthStencilSurface(width, height, this->d3dpp.AutoDepthStencilFormat,
			this->d3dpp.MultiSampleType, this->d3dpp.MultiSampleQuality, TRUE, &depthBuffer->surface, nullptr)))
		{
			delete depthBuffer;
			return nullptr;
		}
		return depthBuffer;
	}

	void Application::SetBlendMode(BlendMode blendMode)
	{
		this->renderState.blendMode = blendMode;
		switch (blendMode)
		{
		case BlendMode::ALPHA_BLEND:
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;
		case BlendMode::ADD:
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
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
		IDirect3DSurface9 *surface;
		device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);

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

		D3DXSaveSurfaceToFile(path.GetBuffer(), D3DXIFF_JPG, surface, NULL, NULL);
		surface->Release();
	}

	bool Application::IsDeviceLost()
	{
		return device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET;
	}

	bool Application::NeedResetDevice()
	{
		return device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET;
	}

	void Application::OnLostDevice()
	{
		AssetManager::GetInstance()->OnLostDevice();
	}

	void Application::OnResetDevice()
	{
		device->Reset(&d3dpp);

		ResetDeviceState();

		AssetManager::GetInstance()->OnResetDevice();
	}

	bool Application::IsMinimized()
	{
		if (IsIconic(hWnd))
		{
			return true;
		}
		return false;
	}

	void Application::EnableDepthTest(bool value)
	{
		this->renderState.isDepthTestEnabled = value;
		this->device->SetRenderState(D3DRS_ZENABLE, value);
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