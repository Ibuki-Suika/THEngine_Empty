#include "THMeshRenderer.h"
#include "THShadowMap.h"
#include "THRenderQueue.h"
#include <Platform\THApplication.h>
#include <Asset\THAssetManager.h>
#include <Asset\THShaderStock.h>
#include <Asset\THFloatTexture.h>
#include <Core\THCamera.h>
#include <Core\THEnvironment.h>
#include <Core\3D\THMesh.h>
#include <Core\THGame.h>
#include <Core\THConfig.h>

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

		return renderer;
	}

	void MeshRenderer::SetupRenderState()
	{
		auto app = Application::GetInstance();
		auto device = app->GetDevice();
		auto renderState = app->GetRenderState();

		if (renderState->IsLightingEnabled())
		{
			device->SetRenderState(D3DRS_LIGHTING, true);
		}
		else
		{
			device->SetRenderState(D3DRS_LIGHTING, false);
		}
	}

	void MeshRenderer::SetupShaderParams(Mesh* mesh)
	{
		auto app = Application::GetInstance();
		auto meshShader = ShaderStock::GetInstance()->GetMeshShader();

		SetupWorldTransform(mesh);

		Material material = mesh->material;

		float argb[4];
		argb[0] = mesh->alpha;
		argb[1] = mesh->color.x;
		argb[2] = mesh->color.y;
		argb[3] = mesh->color.z;

		auto renderState = app->GetRenderState();

		Matrix mv = renderState->GetWorldMatrix() * renderState->GetViewMatrix();
		Matrix normalMatrix = mv;
		normalMatrix.Inverse();
		normalMatrix.Transpose();

		if (mesh->mesh == nullptr)
		{
			meshShader->SetTexture("tex", material.texture);
			meshShader->SetBoolean("hasTexture", true);
		}

		meshShader->Use();

		meshShader->SetFloatArray("argb", argb, 4);
		meshShader->SetBoolean("fogEnable", renderState->IsFogEnabled());
		meshShader->SetValue("fog", (void*)&renderState->GetFog(), sizeof(Fog));
		meshShader->SetMatrix("worldMatrix", renderState->GetWorldMatrix());
		meshShader->SetMatrix("mvMatrix", mv);
		meshShader->SetMatrix("normalMatrix", normalMatrix);
		meshShader->SetMatrix("projection", renderState->GetProjectionMatrix());
	}

	void MeshRenderer::DrawMesh(Mesh* mesh)
	{
		auto meshShader = ShaderStock::GetInstance()->GetMeshShader();

		meshShader->UsePass(0);
		mesh->DrawGeometry();
		meshShader->EndPass();
	}

	void MeshRenderer::ShadeWithAmbient(Mesh* mesh, const Vector4f& ambient)
	{
		auto meshShader = ShaderStock::GetInstance()->GetMeshShader();

		meshShader->SetTechnique("Ambient");
		meshShader->SetFloat4("ambientLight", ambient);
		DrawMesh(mesh);
	}

	void MeshRenderer::ShadeWithLight(Mesh* mesh)
	{
	}

	void MeshRenderer::Shade(Mesh* mesh)
	{
		auto app = Application::GetInstance();
		auto renderState = app->GetRenderState();

		if (Game::GetInstance()->GetConfig()->useLighting && renderState->IsLightingEnabled())
		{
			if (isRenderingAmbient)
			{
				ShadeWithAmbient(mesh, renderState->GetAmbientLight());
			}
			else
			{
				ShadeWithLight(mesh);
			}
		}

		else
		{
			DrawMesh(mesh);
		}
	}

	void MeshRenderer::Render(GameObject* object)
	{
		Mesh* mesh = (Mesh*)object;
		auto app = Application::GetInstance();

		SetupShaderParams(mesh);

		Shade(mesh);
	}

	void MeshRenderer::RenderObjects(RenderQueue* renderQueue)
	{
		auto objects = renderQueue->GetObjects();
		auto iter = objects->GetIterator();
		while (iter->HasNext())
		{
			Render(iter->Next());
		}
	}

	//////////////////////////////////////////////
	DirectionalLightRenderer::DirectionalLightRenderer()
	{
	}

	DirectionalLightRenderer::~DirectionalLightRenderer()
	{
	}

	DirectionalLightRenderer* DirectionalLightRenderer::Create()
	{
		DirectionalLightRenderer* renderer = new DirectionalLightRenderer();

		return renderer;
	}

	void DirectionalLightRenderer::ShadeWithLight(Mesh* mesh)
	{
		struct DirectionalLight_For_Render
		{
			Vector4f color;
			Vector3f direction;
		};

		auto app = Application::GetInstance();
		auto renderState = app->GetRenderState();
		auto meshShader = ShaderStock::GetInstance()->GetMeshShader();

		DirectionalLight_For_Render lightForRender;
		lightForRender.color.x = this->currentLight->GetColor().x;
		lightForRender.color.y = this->currentLight->GetColor().y;
		lightForRender.color.z = this->currentLight->GetColor().z;
		lightForRender.color.w = 1.0f;

		DirectionalLight* dirLight = (DirectionalLight*)this->currentLight;
		Vector4f lightDirInView;
		lightDirInView.x = dirLight->GetDirection().x;
		lightDirInView.y = dirLight->GetDirection().y;
		lightDirInView.z = dirLight->GetDirection().z;
		lightDirInView.w = 0;
		lightDirInView *= renderState->GetViewMatrix();
		lightForRender.direction.x = lightDirInView.x;
		lightForRender.direction.y = lightDirInView.y;
		lightForRender.direction.z = lightDirInView.z;
		lightForRender.direction = lightForRender.direction.Normalize();

		meshShader->SetTechnique("Directional");
		meshShader->SetValue("directionalLight", &lightForRender, sizeof(DirectionalLight_For_Render));

		float znear = renderState->GetCamera()->GetZNear();
		float zfar = renderState->GetCamera()->GetZFar();
		meshShader->SetFloat("znear", znear);
		meshShader->SetFloat("zfar", zfar);
		meshShader->SetFloat("blendBand", (znear - zfar) / 30);
		if (this->shadowMapNear)
		{
			meshShader->SetTexture("shadowMapNear", this->shadowMapNear->GetShadowMap());
			meshShader->SetTexture("shadowMapMid", this->shadowMapMid->GetShadowMap());
			meshShader->SetTexture("shadowMapFar", this->shadowMapFar->GetShadowMap());
			meshShader->SetInt("shadowMapWidth", this->shadowMapNear->GetShadowMap()->GetWidth());
			meshShader->SetInt("shadowMapHeight", this->shadowMapNear->GetShadowMap()->GetHeight());
		}
		if (this->currentLight)
		{
			Matrix lightVPNear = this->shadowMapNear->GetLightView() * this->shadowMapNear->GetLightProjection();
			meshShader->SetMatrix("lightVPNear", lightVPNear);

			Matrix lightVPMid = this->shadowMapMid->GetLightView() * this->shadowMapMid->GetLightProjection();
			meshShader->SetMatrix("lightVPMid", lightVPMid);

			Matrix lightVPFar = this->shadowMapFar->GetLightView() * this->shadowMapFar->GetLightProjection();
			meshShader->SetMatrix("lightVPFar", lightVPFar);
		}

		DrawMesh(mesh);
	}
}