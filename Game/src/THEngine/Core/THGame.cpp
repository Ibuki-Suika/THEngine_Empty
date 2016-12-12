#include "THGame.h"

using namespace THEngine;

Game* Game::instance = nullptr;

Game::Game()
{
	ASSERT(instance == nullptr);
	instance = this;

	app = nullptr;
	scene = nullptr;
	nextScene = nullptr;

	lastTime = currentTime = -1;
	frameCount = 0;
	showFPS = true;

	spriteQueue = nullptr;
	spriteRenderer = nullptr;
	assetManager = nullptr;
	defaultFont = nullptr;
	input = nullptr;
}

Game::~Game()
{
	ASSERT(this == instance);
	
	OnShutdown();
}

Game* Game::GetInstance()
{
	if (instance == NULL)
	{
		instance = new Game();
	}
	return instance;
}

bool Game::CreateGame(int width, int height, bool fullScreen, String title, 
	int bigIcon, int smallIcon)
{
	this->width = width;
	this->height = height;
	this->fullScreen = fullScreen;
	this->title = title;

	exceptionManager = ExceptionManager::GetInstance();

	app = new Application();
	if(app->Init(width,height,fullScreen,title,bigIcon,smallIcon) == false)
	{
		return false;
	}
	app->Retain();

	assetManager = AssetManager::Create(app);
	
	spriteQueue = new SpriteRenderQueue();
	spriteQueue->Retain();

	normalQueue = new NormalRenderQueue();
	normalQueue->Retain();

	eventSystem = EventSystem::Create();

	spriteRenderer = SpriteRenderer::Create(app);
	if (spriteRenderer == nullptr)
	{
		auto exception = exceptionManager->GetException();
		auto newException = new Exception((String)"创建SpriteRenderer失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}

	particle3DRenderer = Particle3DRenderer::Create();
	if (particle3DRenderer == nullptr)
	{
		auto exception = exceptionManager->GetException();
		auto newException = new Exception((String)"创建Particle3DRenderer失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}
	particle3DRenderer->Retain();


	meshRenderer = MeshRenderer::Create();
	if (meshRenderer == nullptr)
	{
		auto exception = exceptionManager->GetException();
		auto newException = new Exception((String)"创建MeshRenderer失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}
	meshRenderer->Retain();

	defaultFont = Font::CreateFontFromFile("res/font/font-fps-opensans.png","res/font/font-fps-opensans.txt");
	if (defaultFont == nullptr)
	{
		auto exception = exceptionManager->GetException();
		auto newException = new Exception((String)"创建defaultFont失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}

	input = Input::Create(app);
	if (input == nullptr)
	{
		auto exception = exceptionManager->GetException();
		auto newException = new Exception((String)"初始化DirectInput失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}
	input->Retain();

	audio = Audio::GetInstance();
	if (audio == nullptr)
	{
		auto exception = exceptionManager->GetException();
		auto newException = new Exception((String)"初始化DirectSound失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}
	audio->Retain();

	return true;
}

int Game::Run()
{
	while (!app->NeedQuit())
	{
		if (app->IsDeviceLost())
		{
			if (app->NeedResetDevice())
			{
				app->OnLostDevice();
				app->OnResetDevice();
			}
		}

		if (app->IsMinimized())
		{
			if (enterBackground == false)
			{
				OnEnterBackground();
				enterBackground = true;
			}
		}
		else if (enterBackground)
		{
			OnReturnToForeground();
			enterBackground = false;
		}

		if(enterBackground == false)
		{
			CalcFPS();
			Update();
			Draw();

			if (nextScene)
			{
				if (scene)
				{
					scene->OnSceneChanged();
				}
				SetScene(nextScene);
				nextScene->OnLoad();
				nextScene = nullptr;
			}
		}

		app->DealWithMessage();
	}

	int returnCode = app->GetReturnCode();
	Shutdown();
	return returnCode;
}

void Game::OnEnterBackground()
{

}

void Game::OnReturnToForeground()
{

}

void Game::Update()
{
	input->Update();
	eventSystem->Update();

	if (scene && scene->IsPaused() == false)
	{
		scene->Update();
	}
}

void Game::Draw()
{
	app->ClearBuffer();

	app->BeginRender();

	if (scene)
	{
		scene->Draw();
	}
	
	if (showFPS)
	{
		DrawFPS();
	}

	app->EndRender();
	
	app->SwapBuffers();
}

void Game::SetScene(Scene* scene)
{
	TH_SAFE_RELEASE(this->scene);
	scene->Retain();
	this->scene = scene;
}

void Game::LoadScene(Scene* scene)
{
	nextScene = scene;
}

void Game::Render()
{
	spriteQueue->Render();
	spriteQueue->Clear();

	normalQueue->Render();
	normalQueue->Clear();
}


void Game::SendToRenderQueue(RenderQueueType type, GameObject* obj)
{
	switch (type)
	{
	case SPRITE:
		spriteQueue->Add(obj);
		break;
	case NORMAL:
		normalQueue->Add(obj);
		break;
	}
}

void Game::CalcFPS()
{
	currentTime = GetTickCount();
	frameCount++;
	if (lastTime < 0)
	{
		lastTime = currentTime;
		return;
	}
	if (currentTime - lastTime >= 1000)
	{
		fps = (float)frameCount / (currentTime - lastTime) * 1000;
		lastTime = currentTime;
		frameCount = 0;
	}
}

void Game::DrawFPS()
{
	app->SetViewport(0, 0, width, height);
	app->SetOrtho(0, 0, width, height, 0, TH_MAX_Z);

	char buffer[10];
	sprintf(buffer, "%.2f", fps);
	defaultFont->DrawString(buffer, width - 65, 30);
}

void Game::Shutdown()
{
	OnShutdown();

	TH_SAFE_RELEASE(scene);

	TH_SAFE_RELEASE(spriteQueue);
	TH_SAFE_RELEASE(normalQueue);
	TH_SAFE_RELEASE(spriteRenderer);
	TH_SAFE_RELEASE(particle3DRenderer);
	TH_SAFE_RELEASE(meshRenderer);
	
	TH_SAFE_RELEASE(eventSystem);
	TH_SAFE_RELEASE(defaultFont);
	
	TH_SAFE_RELEASE(input);
	TH_SAFE_RELEASE(assetManager);
	TH_SAFE_RELEASE(audio);
	TH_SAFE_RELEASE(app);

	delete Logger::GetInstance();
	delete exceptionManager;
}

void Game::OnShutdown()
{

}