#include "THInput.h"
#include "THApplication.h"

using namespace THEngine;

Input::~Input()
{
	TH_SAFE_RELEASE(input);
}

bool Input::Init()
{
	auto app = Application::GetInstance();
	HRESULT hr;
	hr = DirectInput8Create(app->hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&input, NULL);
	if (FAILED(hr))
	{
		THMessageBox(TEXT("Create DirectInput failed!"));
		return false;
	}

	hr = input->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(hr))
	{
		THMessageBox(TEXT("Create keyboard device failed!"));
		return false;
	}

	hr = keyboard->SetCooperativeLevel(app->hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		THMessageBox(TEXT("Set keyboard cooperative level failed!"));
		return false;
	}

	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		THMessageBox(TEXT("Set keyboard data format failed!"));
		return false;
	}

	hr = keyboard->Acquire();
	if (FAILED(hr))
	{
		THMessageBox(TEXT("Acquire keyboard failed!"));
		return false;
	}
	return true;
}

void Input::Update()
{
	GetKeyboardState();
}

int Input::GetKeyboardState()
{
	HRESULT hr;
	while ((hr = keyboard->GetDeviceState(256, &keystate)) == DIERR_INPUTLOST)
	{
		hr = keyboard->Acquire();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (FAILED(hr))
		return hr;
	return S_OK;
}

bool Input::KeyDown(int key)
{
	return (DIKEYDOWN(keystate, key) != 0);
}