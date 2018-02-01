#include "THGame.h"
#include "THFont.h"
#include "THSprite.h"
#include "THDataStack.h"
#include "THScene.h"
#include "THConfig.h"
#include <Scheduling\THScheduler.h>
#include <Scheduling\THFrameTimer.h>
#include <Platform\THApplication.h>
#include <Platform\THDevice.h>
#include <Platform\THInput.h>
#include <Platform\THAudio.h>
#include <Platform\THSystemClock.h>
#include <Renderer\THRenderPipeline.h>
#include <Asset\THAssetManager.h>
#include <Asset\THShaderStock.h>
#include <UI\THEventSystem.h>
#include <Async\THAsyncLoader.h>

using namespace THEngine;

Ptr<Game> Game::instance;
std::mutex Game::mtx_instance;

Game::Game()
{
	TH_LOCK(mtx_instance)
	{
		if (instance != nullptr)
			throw std::logic_error("Trying to construct more than one instance of Game.");
		instance = this;
	}

	app = nullptr;
	scene = nullptr;
	nextScene = nullptr;

	lastTime = currentTime = -1;
	frameCount = 0;
	showFPS = true;

	assetManager = nullptr;
	defaultFont = nullptr;
	input = nullptr;
}

Game::~Game()
{
	OnShutdown();
}

Ptr<Game> Game::GetInstance()
{
	return instance;
}

bool Game::CreateGame(int width, int height, bool fullScreen, const String& title,
	int bigIcon, int smallIcon)
{
	Config config;

	config.width = width;
	config.height = height;
	config.fullScreen = fullScreen;
	config.title = title;

	return CreateGame(config, bigIcon, smallIcon);
}

bool Game::CreateGame(const Config& config, int bigIcon, int smallIcon)
{
	exceptionManager = ExceptionManager::GetInstance();

	this->config = new Config(config);

	app = Application::GetInstance();
	if (app->Init(*this->config, bigIcon, smallIcon) == false)
	{
		return false;
	}

	assetManager = AssetManager::GetInstance();
	if (assetManager->Init() == false)
	{
		auto exception = exceptionManager->GetException();
		auto newException = Ptr<Exception>::New((String)"创建AssetManager失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}

	shaderStock = ShaderStock::GetInstance();
	if (shaderStock->Init() == false)
	{
		auto exception = exceptionManager->GetException();
		auto newException = Ptr<Exception>::New((String)"创建ShaderStock失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}

	eventSystem = EventSystem::GetInstance();
	if (eventSystem->Init() == false)
	{
		auto exception = exceptionManager->GetException();
		auto newException = Ptr<Exception>::New((String)"创建EventSystem失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}

	pipeline = RenderPipeline::Create();
	if (pipeline == nullptr)
	{
		auto exception = exceptionManager->GetException();
		auto newException = Ptr<Exception>::New((String)"创建RenderPipleline失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}

	defaultFont = Font::CreateFontFromFile("res/font/font-fps-opensans.png", "res/font/font-fps-opensans.txt");
	if (defaultFont == nullptr)
	{
		auto exception = exceptionManager->GetException();
		auto newException = Ptr<Exception>::New((String)"创建defaultFont失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}

	input = Input::GetInstance();
	if (input->Init() == false)
	{
		auto exception = exceptionManager->GetException();
		auto newException = Ptr<Exception>::New((String)"初始化DirectInput失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}

	audio = Audio::GetInstance();
	if (audio->Init() == false)
	{
		auto exception = exceptionManager->GetException();
		auto newException = Ptr<Exception>::New((String)"初始化DirectSound失败。原因是：\n" + exception->GetInfo());
		exceptionManager->PushException(newException);
		return false;
	}

	dataStack = DataStack::GetInstance();

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	this->timerFrequency = frequency.QuadPart;

	return true;
}

int Game::GetWidth() const
{
	return this->config->width;
}

int Game::GetHeight() const
{
	return this->config->height;
}

const String& Game::GetTitle() const
{
	return this->config->title;
}

int Game::Run()
{
	auto device = app->GetDevice();
	while (!app->NeedQuit())
	{
		if (device->IsDeviceLost())
		{
			if (device->NeedResetDevice())
			{
				device->OnLostDevice();
				device->OnResetDevice();
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

		if (enterBackground == false)
		{
			if (this->speedCounter < 1.0f)
			{
				this->speedCounter += this->speedReciprocal;
				Update();
			}
			if (this->speedCounter >= 1.0f)
			{
				this->speedCounter -= 1.0f;
				if (config->useVSync)
				{
					CalcFPS();
					Draw();
				}
				else
				{
					while (true)
					{
						Time currentTime = SystemClock::GetInstance()->GetTime();
						long long deltaTime = currentTime.ToMicroSecond() - this->lastTimeStamp.ToMicroSecond();
						if (deltaTime > 1000000.0f / config->fps)
						{
							CalcFPS();
							Draw();
							this->lastTimeStamp = currentTime;
							break;
						}
					}
				}
			}

			if (nextScene != nullptr)
			{
				if (scene != nullptr)
				{
					scene->OnDestroy();
				}
				SetScene(nextScene);
				nextScene = nullptr;
			}
		}

		app->DealWithMessage();
	}

	int returnCode = app->GetReturnCode();
	Shutdown();
	return returnCode;
}

void Game::Quit()
{
	app->Quit();
}

void Game::OnEnterBackground()
{
}

void Game::OnReturnToForeground()
{
}

void Game::Update()
{
	EngineObject::Update();

	input->Update();
	eventSystem->Update();
	audio->Update();

	if (scene != nullptr && scene->IsPaused() == false)
	{
		scene->Update();
	}

	assetManager->Update();
}

void Game::Draw()
{
	auto device = app->GetDevice();
	device->ClearBuffer();

	device->BeginRender();
	if (scene != nullptr)
	{
		scene->Draw();
	}
	if (showFPS)
	{
		DrawFPS();
	}
	device->EndRender();

	device->SwapBuffers();
}

inline void Game::SetScene(Ptr<Scene> scene)
{
	this->scene = scene;
}

void Game::LoadScene(Ptr<Scene> scene)
{
	nextScene = scene;
	if (scene->loaded == false)
	{
		nextScene->OnLoad(nullptr);
	}
}

void Game::LoadSceneAsync(Ptr<Scene> scene)
{
	if (scene->loaded)
	{
		return;
	}
	AsyncLoader::Load(scene.Get(), [this, scene]()
	{
		this->nextScene = scene;
	});
}

void Game::LoadSceneAsync(Ptr<Scene> scene, int delay, const std::function<void()>& onLoadCompleted)
{
	if (scene->loaded)
	{
		return;
	}
	AsyncLoader::Load(scene.Get(), [this, scene, delay, onLoadCompleted]()
	{
		Ptr<FrameTimer> timer = Ptr<FrameTimer>::New();
		timer->SetFrame(delay);
		timer->run = [this, scene]() {
			this->nextScene = scene;
		};
		this->GetScheduler()->AddFrameTimer(timer);
		onLoadCompleted();
	});
}

Ptr<AsyncInfo> Game::LoadSceneAsyncWithInfo(Ptr<Scene> scene, bool autoChange)
{
	if (scene->loaded)
	{
		return nullptr;
	}
	if (autoChange)
	{
		return AsyncLoader::LoadWithInfo(scene.Get(), [this, scene]()
		{
			this->nextScene = scene;
		});
	}
	return AsyncLoader::LoadWithInfo(scene.Get());
}

void Game::CalcFPS()
{
	this->currentTime = GetTickCount();
	frameCount++;
	if (lastTime < 0)
	{
		lastTime = currentTime;
		return;
	}
	if (currentTime - lastTime >= 1000)
	{
		fps = (double)frameCount / (currentTime - lastTime) * 1000;
		lastTime = currentTime;
		frameCount = 0;
	}
}

void Game::DrawFPS()
{
	auto device = app->GetDevice();
	device->SetViewport(0, 0, GetWidth(), GetHeight());
	device->SetOrtho(0, 0, GetWidth(), GetHeight(), 0, TH_MAX_Z);

	char buffer[10];
	sprintf(buffer, "%.2f", fps);
	defaultFont->DrawString(buffer, GetWidth() - 65, 30);
}

void Game::Shutdown()
{
	OnShutdown();

	TH_SAFE_DELETE(config);

	pipeline = nullptr;
	scene = nullptr;
	eventSystem = nullptr;
	defaultFont = nullptr;
	input = nullptr;
	shaderStock = nullptr;
	assetManager = nullptr;
	audio = nullptr;
	app = nullptr;
	dataStack = nullptr;

	EventSystem::DestroyInstance();
	Input::DestroyInstance();
	ShaderStock::DestroyInstance();
	AssetManager::DestroyInstance();
	Audio::DestroyInstance();
	Application::DestroyInstance();
	DataStack::DestroyInstance();

	delete Logger::GetInstance();
}

void Game::OnShutdown()
{
}