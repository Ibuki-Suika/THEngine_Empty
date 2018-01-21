#ifndef THGAME_H
#define THGAME_H

#include <Common\THCommon.h>
#include <Scheduling\THTime.h>
#include <Platform\THCoreDumper.h>

namespace THEngine
{
	class Application;
	class Input;
	class Audio;
	class Scene;
	class Font;
	class Sprite;
	class DataStack;
	class RenderPipeline;
	class AssetManager;
	class EventSystem;
	class Font;
	class ShaderStock;
	class RenderPipeline;
	class AsyncInfo;
	struct Config;

	class Game : public EngineObject
	{
	private:
		Config* config = nullptr;

		static Ptr<Game> instance;
		static std::mutex mtx_instance; //for GetInstance()

		bool enterBackground = false;

		float fps;
		int frameCount;
		bool showFPS;

		//game speed control
		float speed = 1.0f;          //1表示原速，2表示2倍速
		float speedReciprocal = 1;   //速率的倒数
		float speedCounter = 0;

		//fps control
		Time lastTimeStamp;

		long long lastTime, currentTime, timerFrequency;

		CoreDumper coreDumper;

		Ptr<Application> app;
		Ptr<Scene> scene;
		Ptr<Scene> nextScene;
		Ptr<RenderPipeline> pipeline;
		Ptr<AssetManager> assetManager;
		Ptr<ShaderStock> shaderStock;
		Ptr<EventSystem> eventSystem;
		Ptr<Font> defaultFont;
		Ptr<Input> input;
		Ptr<Audio> audio;
		Ptr<ExceptionManager> exceptionManager;
		Ptr<DataStack> dataStack;

	protected:
		void CalcFPS();
		void DrawFPS();

	public:
		Game();
		virtual ~Game();

		static Ptr<Game> GetInstance();

		virtual void Update() override;
		virtual void Draw() override;
		virtual void OnShutdown();
		virtual void OnEnterBackground();
		virtual void OnReturnToForeground();

		int Run();

		void Shutdown();

		void Quit();

		bool CreateGame(int width, int height, bool fullScreen, const String& title, int bigIcon, int smallIcon);

		bool CreateGame(const Config& config, int bigIcon, int smallIcon);

		void SetScene(Ptr<Scene> scene);

		void LoadScene(Ptr<Scene> scene);

		void LoadSceneAsync(Ptr<Scene> scene);

		void LoadSceneAsync(Ptr<Scene> scene, int delay, const std::function<void()>& onLoadCompleted);

		Ptr<AsyncInfo> LoadSceneAsyncWithInfo(Ptr<Scene> scene, bool autoChange);

		inline void SetSpeed(float speed)
		{
			this->speed = speed;
			this->speedReciprocal = 1.0f / speed;
		}
		inline float GetSpeed() const { return this->speed; }

		inline Config* GetConfig() const { return this->config; }

		inline Ptr<Application> GetApplication() const { return app; }

		inline Ptr<RenderPipeline> GetRenderPipeline() const { return pipeline; }

		inline Ptr<AssetManager> GetAssetManager() const
		{
			return assetManager;
		}

		int GetWidth() const;
		int GetHeight() const;

		inline Ptr<Scene> GetScene() const { return scene; }

		const String& GetTitle() const;
	};
}

#endif