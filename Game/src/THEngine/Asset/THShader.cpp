#include "THShader.h"
#include "THTexture.h"
#include "THCubeMap.h"
#include <Platform\THDevice.h>

namespace THEngine
{
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
		auto device = Device::GetInstance()->GetD3DDevice();

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

	void Shader::SetTexture(char* textureName, Ptr<Texture> texture)
	{
		effect->SetTexture(textureName, texture->texImpl->texture);
	}

	void Shader::SetCubeMap(char* textureName, Ptr<CubeMap> cubeMap)
	{
		effect->SetTexture(textureName, cubeMap->impl->cubeTexture);
	}

	void Shader::SetTechnique(char* technique)
	{
		auto renderState = Device::GetInstance()->GetRenderState();
		if (renderState->shader == this)
		{
			End();
			effect->SetTechnique(technique);
			Use();
		}
		else
		{
			effect->SetTechnique(technique);
		}
	}

	void Shader::Use()
	{
		auto renderState = Device::GetInstance()->GetRenderState();
		if (renderState->shader != this)
		{
			if (renderState->shader != nullptr)
			{
				renderState->shader->End();
			}
			renderState->shader = this;
			this->effect->Begin(&passNum, 0);
			this->currentPass = -1;
		}
	}

	void Shader::End()
	{
		auto renderState = Device::GetInstance()->GetRenderState();
		if (renderState->shader == this)
		{
			if (this->currentPass >= 0)
			{
				EndPass();
			}
			this->effect->End();
			renderState->shader = nullptr;
		}
	}

	void Shader::UsePass(unsigned int pass)
	{
		if (currentPass != pass)
		{
			EndPass();
			currentPass = pass;
			effect->BeginPass(pass);
		}
	}

	void Shader::EndPass()
	{
		if (currentPass != -1)
		{
			currentPass = -1;
			effect->EndPass();
		}
	}
}