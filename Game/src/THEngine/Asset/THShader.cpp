#include "THShader.h"
#include "../Platform/THApplication.h"

using namespace THEngine;

Shader::Shader()
{
	effect = nullptr;
}

Shader::~Shader()
{
	TH_SAFE_RELEASE(effect);
}

void Shader::OnLostDevice()
{
	TH_SAFE_RELEASE(effect);
}

void Shader::OnResetDevice()
{
	auto device = Application::GetInstance()->GetDevice();

	ID3DXBuffer *error;
	if (FAILED(D3DXCreateEffectFromFile(device, path.GetBuffer(), NULL, NULL,
		D3DXSHADER_DEBUG, NULL, &effect, &error)))
	{
		if (error)
		{
			String message((char*)error->GetBufferPointer());
			THMessageBox(message);
		}
		else
		{
			THMessageBox((String)"无法打开文件:" + path);
		}
	}
}