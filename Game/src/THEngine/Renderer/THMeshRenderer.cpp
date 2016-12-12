#include "THMeshRenderer.h"
#include "../Platform/THApplication.h"
#include "../Asset/THAssetManager.h"

namespace THEngine
{
	MeshRenderer::MeshRenderer()
	{

	}

	MeshRenderer::~MeshRenderer()
	{

	}

	MeshRenderer* MeshRenderer::Create()
	{
		MeshRenderer* renderer = new MeshRenderer();
		if (renderer)
		{
			auto assetManager = AssetManager::GetInstance();

			renderer->meshShader = assetManager->CreateShaderFromFile("fx/mesh.fx");
			if (renderer->meshShader)
			{
				renderer->meshShader->SetTechnique("Mesh");
			}
			else
			{
				delete renderer;
				return nullptr;
			}

		}
		return renderer;
	}

	void MeshRenderer::CalcWorldTransform(Mesh* mesh)
	{
		D3DXMATRIX transform;
		D3DXMatrixIdentity(&transform);

		D3DXMATRIX temp;
		D3DXMatrixScaling(&temp, mesh->GetScale().x, mesh->GetScale().y, mesh->GetScale().z);
		transform *= temp;

		D3DXMatrixRotationQuaternion(&temp, &mesh->rotation3D);
		transform *= temp;

		D3DXMatrixTranslation(&temp, floor(0.5f + mesh->position.x), floor(0.5f + mesh->position.y), mesh->position.z);
		transform *= temp;

		Application::GetInstance()->SetWorldTransform(&transform);
	}

	void MeshRenderer::SetupRenderState()
	{
		auto app = Application::GetInstance();
		auto device = app->GetDevice();
		auto renderState = app->GetRenderState();

		if (renderState->lightingEnable)
		{
			device->SetRenderState(D3DRS_LIGHTING, true);
		}
		else
		{
			device->SetRenderState(D3DRS_LIGHTING, false);
		}
	}

	void MeshRenderer::Render(RenderObject* object)
	{
		Mesh* mesh = (Mesh*)object;
		auto app = Application::GetInstance();

		CalcWorldTransform(mesh);

		Material material = mesh->material;

		float argb[4];
		argb[0] = mesh->alpha;
		argb[1] = mesh->color.x;
		argb[2] = mesh->color.y;
		argb[3] = mesh->color.z;

		auto renderState = app->GetRenderState();

		D3DXMATRIX mv = renderState->world * renderState->view;
		D3DXMATRIX normalMatrix;
		D3DXMatrixInverse(&normalMatrix, nullptr, &mv);
		D3DXMatrixTranspose(&normalMatrix, &normalMatrix);

		meshShader->Begin();

		meshShader->SetFloatArray("argb", argb, 4);
		meshShader->SetBoolean("fogEnable", renderState->fogEnable);
		meshShader->SetValue("fog", &renderState->fog, sizeof(Fog));
		meshShader->SetMatrix("mvMatrix", &mv);
		meshShader->SetMatrix("normalMatrix", &normalMatrix);
		meshShader->SetMatrix("projection", &renderState->projection);

		
		if (mesh->mesh)
		{
			DrawD3DMesh(mesh->mesh);
		}
		else
		{
			meshShader->SetTexture("tex", material.texture);
			meshShader->SetBoolean("hasTexture", true);

			meshShader->BeginPass(0);
			switch (mesh->primitiveType)
			{
			case Mesh::TRIANGLE_LIST:
				app->GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, mesh->vertexCount / 3);
				break;
			case Mesh::TRIANGLE_STRIP:
				app->GetDevice()->SetStreamSource(0, mesh->vertexBuffer, 0, sizeof(MeshVertex));
				app->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, mesh->vertexCount - 2);
				break;
			}
			meshShader->EndPass();
		}
		
		meshShader->End();
	}

	void MeshRenderer::DrawD3DMesh(Mesh::D3DMesh* mesh)
	{
		for (int i = 0; i < mesh->numMaterials; i++)
		{
			Material& mat = mesh->materialList[i];
			if (mat.texture)
			{
				meshShader->SetTexture("tex", mat.texture);
				meshShader->SetBoolean("hasTexture", true);
			}
			else
			{
				meshShader->SetBoolean("hasTexture", false);
			}
			meshShader->SetValue("material", &mat, sizeof(Material) - sizeof(Texture*));

			meshShader->BeginPass(0);
			mesh->mesh->DrawSubset(i);
			meshShader->EndPass();
		}
	}
}