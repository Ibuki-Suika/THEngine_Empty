#ifndef THGAME_H
#define THGAME_H

#include "../Common/THCommon.h"
#include "../Platform/THApplication.h"
#include "../Platform/THInput.h"
#include "../Platform/THAudio.h"
#include "THScene.h"
#include "THFont.h"
#include "THSprite.h"
#include "../Renderer/THSpriteRenderer.h"
#include "../Renderer/THParticle3DRenderer.h"
#include "../Renderer/THMeshRenderer.h"
#include "../Asset/THAssetManager.h"
#include "../UI/THEventSystem.h"


namespace THEngine
{
	class Font;

	class Game : public EngineObject
	{
	private:
		static Game* instance;
		String title;
		int width, height;
		bool fullScreen;

		bool enterBackground = false;

		float fps;
		int frameCount;
		bool showFPS;
		
		unsigned int lastTime, currentTime;

		Application* app;

		Scene* scene;
		Scene* nextScene;

		RenderQueue* spriteQueue;
		RenderQueue* normalQueue = nullptr;

		SpriteRenderer* spriteRenderer;

		Particle3DRenderer* particle3DRenderer = nullptr;

		MeshRenderer* meshRenderer = nullptr;

		AssetManager* assetManager;

		EventSystem* eventSystem;

		Font* defaultFont;

		Input* input;

		Audio* audio = nullptr;

		ExceptionManager* exceptionManager = nullptr;

	private:
		void CalcFPS();
		void DrawFPS();

	public:
		enum RenderQueueType
		{
			SPRITE,
			NORMAL
		};

	public:
		Game();
		virtual ~Game();

		virtual void Update() override;
		virtual void Draw() override;
		virtual void OnShutdown();
		virtual void OnEnterBackground();
		virtual void OnReturnToForeground();

		int Run();

		void Shutdown();

		inline void Quit()
		{
			app->Quit();
		}

		static Game* GetInstance();

		bool CreateGame(int width, int height, bool fullScreen, String title, int bigIcon, int smallIcon);

		void SetScene(Scene* scene);

		void LoadScene(Scene* scene);

		void Render();

		inline Application* GetApplication() { return app; }

		inline SpriteRenderer* GetSpriteRenderer()
		{
			return spriteRenderer;
		}

		inline Particle3DRenderer* GetParticle3DRenderer()
		{
			return particle3DRenderer;
		}

		inline MeshRenderer* GetMeshRenderer()
		{
			return meshRenderer;
		}

		inline AssetManager* GetAssetManager()
		{
			return assetManager;
		}

		inline int GetWidth() { return width; }

		inline int GetHeight() { return height; }

		inline Scene* GetScene() { return scene; }

		void SendToRenderQueue(RenderQueueType type, GameObject* obj);
	};
}

#endif