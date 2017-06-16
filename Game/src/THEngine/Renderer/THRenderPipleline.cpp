#include "THRenderPipeline.h"
#include "THShadowRenderer.h"
#include "THSpriteRenderer.h"
#include "THParticle3DRenderer.h"
#include "THMeshRenderer.h"
#include "THSkyBoxRenderer.h"
#include "THRenderQueue.h"
#include <Core\THGame.h>
#include <Core\THGameObject.h>
#include <Core\THConfig.h>
#include <Platform\THDevice.h>

namespace THEngine
{
	RenderPipeline::RenderPipeline()
	{
	}

	RenderPipeline::~RenderPipeline()
	{
		TH_SAFE_RELEASE(spriteQueue);
		TH_SAFE_RELEASE(normalQueue);
		TH_SAFE_RELEASE(spriteRenderer);
		TH_SAFE_RELEASE(particle3DRenderer);
		TH_SAFE_RELEASE(meshRenderer);
		TH_SAFE_RELEASE(skyBoxRenderer);
		TH_SAFE_RELEASE(dirShadowRenderer);
		TH_SAFE_RELEASE(dirLightRenderer);
	}

	RenderPipeline* RenderPipeline::Create()
	{
		RenderPipeline* pipeline = new RenderPipeline();
		auto exceptionManager = ExceptionManager::GetInstance();

		pipeline->spriteQueue = new SpriteRenderQueue();
		pipeline->spriteQueue->Retain();

		pipeline->normalQueue = new NormalRenderQueue();
		pipeline->normalQueue->Retain();

		pipeline->spriteRenderer = SpriteRenderer::Create();
		if (pipeline->spriteRenderer == nullptr)
		{
			auto exception = exceptionManager->GetException();
			auto newException = new Exception((String)"创建SpriteRenderer失败。原因是：\n" + exception->GetInfo());
			exceptionManager->PushException(newException);
			delete pipeline;
			return nullptr;
		}
		pipeline->spriteRenderer->Retain();

		pipeline->particle3DRenderer = Particle3DRenderer::Create();
		if (pipeline->particle3DRenderer == nullptr)
		{
			auto exception = exceptionManager->GetException();
			auto newException = new Exception((String)"创建Particle3DRenderer失败。原因是：\n" + exception->GetInfo());
			exceptionManager->PushException(newException);
			delete pipeline;
			return nullptr;
		}
		pipeline->particle3DRenderer->Retain();

		pipeline->meshRenderer = MeshRenderer::Create();
		if (pipeline->meshRenderer == nullptr)
		{
			auto exception = exceptionManager->GetException();
			auto newException = new Exception((String)"创建MeshRenderer失败。原因是：\n" + exception->GetInfo());
			exceptionManager->PushException(newException);
			delete pipeline;
			return nullptr;
		}
		pipeline->meshRenderer->Retain();

		pipeline->dirLightRenderer = DirectionalLightRenderer::Create();
		if (pipeline->dirLightRenderer == nullptr)
		{
			auto exception = exceptionManager->GetException();
			auto newException = new Exception((String)"创建DirectionalLightRenderer失败。原因是：\n" + exception->GetInfo());
			exceptionManager->PushException(newException);
			delete pipeline;
			return nullptr;
		}
		pipeline->dirLightRenderer->Retain();

		pipeline->skyBoxRenderer = SkyBoxRenderer::Create();
		if (pipeline->skyBoxRenderer == nullptr)
		{
			auto exception = exceptionManager->GetException();
			auto newException = new Exception((String)"创建SkyBoxRenderer失败。原因是：\n" + exception->GetInfo());
			exceptionManager->PushException(newException);
			delete pipeline;
			return nullptr;
		}
		pipeline->skyBoxRenderer->Retain();

		pipeline->dirShadowRenderer = DirectionalLightShadowRenderer::Create();
		if (pipeline->dirShadowRenderer == nullptr)
		{
			auto exception = exceptionManager->GetException();
			auto newException = new Exception((String)"创建DirectionalLightShadowRenderer失败。原因是：\n"
				+ exception->GetInfo());
			exceptionManager->PushException(newException);
			delete pipeline;
			return nullptr;
		}
		pipeline->dirShadowRenderer->Retain();

		return pipeline;
	}

	void RenderPipeline::Render()
	{
		spriteQueue->Render();
		spriteQueue->Clear();

		Render3D();
	}

	void RenderPipeline::SendToRenderQueue(RenderQueueType type, GameObject* obj)
	{
		switch (type)
		{
		case SPRITE:
			spriteQueue->Add(obj);
			break;
		case NORMAL:
			normalQueue->Add(obj);
			break;
		default:
			throw std::runtime_error("not implemented.");
		}
	}

	void RenderPipeline::Render3D()
	{
		auto device = Device::GetInstance();
		auto renderState = device->GetRenderState();

		auto config = Game::GetInstance()->GetConfig();
		if (config->useLighting && renderState->IsLightingEnabled())
		{
			this->meshRenderer->SetRenderAmbient(true);
			normalQueue->Render();
			this->meshRenderer->SetRenderAmbient(false);

			device->SetBlendMode(BlendMode::ADD);
			auto lights = renderState->GetLights();
			auto iter = lights->GetIterator();
			while (iter->HasNext())
			{
				auto light = iter->Next();
				RenderWithLight(light);
			}
			device->SetBlendMode(BlendMode::ALPHA_BLEND);
		}
		else
		{
			normalQueue->Render();
		}
		normalQueue->Clear();
	}

	void RenderPipeline::RenderShadowMap(Light* light)
	{
		ShadowRenderer* shadowRenderer = nullptr;
		switch (light->GetType())
		{
		case Light::DIRECTIONAL:
			shadowRenderer = this->dirShadowRenderer;
			this->dirShadowRenderer->SetLight((DirectionalLight*)light);
			break;
		default:
			throw std::logic_error("not implemented");
		}

		shadowRenderer->RenderShadow(normalQueue);
	}

	void RenderPipeline::RenderWithLight(Light* light)
	{
		RenderShadowMap(light);

		switch (light->GetType())
		{
		case Light::DIRECTIONAL:
			this->dirLightRenderer->SetCurrentLight(light);
			this->dirLightRenderer->SetShadowMapNear(this->dirShadowRenderer->GetCascadedShadowMaps()->Get(0));
			this->dirLightRenderer->SetShadowMapMid(this->dirShadowRenderer->GetCascadedShadowMaps()->Get(1));
			this->dirLightRenderer->SetShadowMapFar(this->dirShadowRenderer->GetCascadedShadowMaps()->Get(2));
			this->dirLightRenderer->RenderObjects(normalQueue);
			break;
		default:
			throw std::logic_error("not implemented");
		}
	}
}