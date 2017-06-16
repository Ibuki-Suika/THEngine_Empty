#include "THSkyBoxRenderer.h"
#include <Core\THLayer.h>
#include <Core\THGame.h>
#include <Core\THCamera.h>
#include <Asset\THShaderStock.h>
#include <Asset\THShader.h>
#include <Platform\THDevice.h>

namespace THEngine
{
	SkyBoxRenderer::SkyBoxRenderer()
	{
	}

	SkyBoxRenderer::~SkyBoxRenderer()
	{
		TH_SAFE_RELEASE(vb);
	}

	SkyBoxRenderer* SkyBoxRenderer::Create()
	{
		SkyBoxRenderer* renderer = new SkyBoxRenderer();
		if (renderer)
		{
			auto device = Device::GetInstance()->GetD3DDevice();
			device->CreateVertexBuffer(4 * sizeof(SkyBoxVertex), D3DUSAGE_DYNAMIC,
				SKYBOX_FVF, D3DPOOL_DEFAULT, &renderer->vb, NULL);
			if (renderer->vb == nullptr)
			{
				ExceptionManager::GetInstance()->PushException(new Exception(("´´½¨¶¥µã»º´æÊ§°Ü¡£")));
				delete renderer;
				return nullptr;
			}
		}
		return renderer;
	}

	void SkyBoxRenderer::RenderSkyBox(Camera3D* camera, Layer* layer)
	{
		CubeMap* skyBox = layer->GetSkyBox();

		auto skyBoxShader = ShaderStock::GetInstance()->GetSkyBoxShader();
		auto device = Device::GetInstance();
		auto d3dDevice = device->GetD3DDevice();
		auto renderState = device->GetRenderState();
		auto& viewport = renderState->GetViewport();

		Matrix matrix;
		Matrix::Ortho(&matrix, 0, viewport.width, 0, viewport.height, 0, TH_MAX_Z);
		device->SetProjectionMatrix(matrix);

		Matrix::Identity(&matrix);
		device->SetWorldMatrix(matrix);
		device->SetViewMatrix(matrix);

		SkyBoxVertex* vertices;
		vb->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);
		vertices[0] = SkyBoxVertex(0, 0, 1);
		vertices[1] = SkyBoxVertex(viewport.width, 0, 1);
		vertices[2] = SkyBoxVertex(0, viewport.height, 1);
		vertices[3] = SkyBoxVertex(viewport.width, viewport.height, 1);
		vb->Unlock();

		d3dDevice->SetFVF(SKYBOX_FVF);
		d3dDevice->SetStreamSource(0, this->vb, 0, sizeof(SkyBoxVertex));

		Vector3f lookDir = camera->GetLookAt() - camera->GetPosition();

		skyBoxShader->Use();

		skyBoxShader->SetFloat("fov", camera->GetFov());
		skyBoxShader->SetFloatArray("lookAt", (float*)&lookDir, 3);
		skyBoxShader->SetFloatArray("up", (float*)&camera->GetUp(), 3);
		skyBoxShader->SetCubeMap("skyBox", skyBox);
		skyBoxShader->SetInt("viewportWidth", viewport.width);
		skyBoxShader->SetInt("viewportHeight", viewport.height);
		skyBoxShader->SetMatrix("world", renderState->GetWorldMatrix());
		skyBoxShader->SetMatrix("projection", renderState->GetProjectionMatrix());
		skyBoxShader->SetMatrix("view", renderState->GetViewMatrix());

		skyBoxShader->UsePass(0);
		d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		skyBoxShader->EndPass();

		device->ClearDepthBuffer();
	}
}