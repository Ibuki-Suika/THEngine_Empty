#include "THInput.h"
#include "THApplication.h"

using namespace THEngine;

Input* Input::instance = nullptr;

Input::Input()
{
	input = nullptr;
}

Input::~Input()
{
	TH_SAFE_RELEASE(input);
}

Input* Input::Create(Application* app)
{
	ASSERT(instance == nullptr);

	instance = new Input();

	HRESULT hr;
	hr = DirectInput8Create(app->hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&instance->input, NULL);
	if (FAILED(hr))
	{
		THMessageBox(TEXT("Create DirectInput failed!"));
		delete instance;
		instance = nullptr;
		return instance;
	}

	hr = instance->input->CreateDevice(GUID_SysKeyboard, &instance->keyboard, NULL);
	if (FAILED(hr))
	{
		THMessageBox(TEXT("Create keyboard device failed!"));
		delete instance;
		instance = nullptr;
		return instance;
	}

	hr = instance->keyboard->SetCooperativeLevel(app->hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		THMessageBox(TEXT("Set keyboard cooperative level failed!"));
		delete instance;
		instance = nullptr;
		return instance;
	}

	hr = instance->keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		THMessageBox(TEXT("Set keyboard data format failed!"));
		delete instance;
		instance = nullptr;
		return instance;
	}

	hr = instance->keyboard->Acquire();
	if (FAILED(hr))
	{
		THMessageBox(TEXT("Acquire keyboard failed!"));
		delete instance;
		instance = nullptr;
		return instance;
	}
	return instance;
}

Input* Input::GetInstance()
{
	return instance;
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