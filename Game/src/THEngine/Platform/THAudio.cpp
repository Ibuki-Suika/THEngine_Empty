#include "THAudio.h"
#include "THApplication.h"
#include "External\CWaveFile.h"
#include <External\SDL2\SDL.h>
#include <External\SDL2\SDL_mixer.h>

#pragma comment(lib, "SDL2/SDL2.lib")
#pragma comment(lib, "SDL2/SDL2_mixer.lib")

namespace THEngine
{
	Audio::~Audio()
	{
		soundList.Clear();
		if (masterVoice)
		{
			masterVoice->DestroyVoice();
		}
		TH_SAFE_RELEASE(xaudio);

		CoUninitialize();

		Mix_CloseAudio();
	}

	bool Audio::Init()
	{
		HRESULT hr;

		if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
		{
			return false;
		}

		if (FAILED(hr = XAudio2Create(&xaudio, 0, XAUDIO2_DEFAULT_PROCESSOR)))
			return false;

		if (FAILED(hr = xaudio->CreateMasteringVoice(&masterVoice)))
			return false;

		if (SDL_Init(SDL_INIT_AUDIO) < 0)
			return false;

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			return false;
		}
		Mix_Volume(-1, this->volume / 100.0f * MIX_MAX_VOLUME);

		return true;
	}

	Ptr<Sound> Audio::CreateSound(const String& filePath)
	{
		auto exceptionManager = ExceptionManager::GetInstance();
		Ptr<Sound> sound;

		String ext = filePath.SubString(filePath.LastIndexOf(TCHAR('.')) + 1, filePath.GetLength());
		if (ext == "wav")
		{
			sound = LoadWav(filePath);
		}
		else
		{
			Ptr<Exception> exception = Ptr<Exception>::New("不支持的文件格式。");
			exceptionManager->PushException(exception);
		}

		if (sound != nullptr)
		{
			sound->SetVolume(volume);
			soundList.Add(sound);
		}
		else
		{
			auto exception = exceptionManager->GetException();
			if (exception != nullptr)
			{
				THMessageBox((String)"无法加载音频文件：" + filePath + "原因是：\n" + exception->GetInfo());
				exceptionManager->SetHandled();
			}
		}
		return sound;
	}

	Ptr<Sound> Audio::LoadWav(const String& filePath)
	{
		Ptr<Sound> sound = Ptr<Sound>::New();
		auto exceptionManager = ExceptionManager::GetInstance();

		CWaveFile wave;
		WAVEFORMATEX wf;

		//the windows API don't accept a const string buffer as parameter, so we cast to non-const
		LPWSTR szFileName = const_cast<LPWSTR>(filePath.GetBuffer());
		if (FAILED(wave.Open(szFileName, &wf, WAVEFILE_READ)))
		{
			Ptr<Exception> exception = Ptr<Exception>::New("无法打开文件。文件路径可能不正确，或文件可能已损坏。");
			exceptionManager->PushException(exception);
			return nullptr;
		}

		sound->callback = new Sound::SoundCallback(sound.Get());

		if (FAILED(xaudio->CreateSourceVoice(&sound->sourceVoice, wave.GetFormat(), 0, 2.0f, sound->callback)))
		{
			Ptr<Exception> exception = Ptr<Exception>::New("无法创建音频缓存。");
			exceptionManager->PushException(exception);
			return nullptr;
		}

		XAUDIO2_BUFFER* buffer = &sound->buffer;
		ZeroMemory(buffer, sizeof(XAUDIO2_BUFFER));
		buffer->Flags = XAUDIO2_END_OF_STREAM;
		buffer->pAudioData = new BYTE[wave.GetSize()];

		DWORD readSize;
		if (FAILED(wave.Read((BYTE*)buffer->pAudioData, wave.GetSize(), &readSize)))
		{
			Ptr<Exception> exception = Ptr<Exception>::New("无法解析文件。文件可能已损坏。");
			exceptionManager->PushException(exception);
			return nullptr;
		}

		buffer->AudioBytes = readSize;

		return sound;
	}

	Ptr<Music> Audio::CreateMusic(const String& filePath)
	{
		auto exceptionManager = ExceptionManager::GetInstance();
		Ptr<Music> music = Ptr<Music>::New();

		char path[256];
		WideCharToMultiByte(CP_ACP, 0, filePath.GetBuffer(), -1, path, sizeof(path), NULL, NULL);
		music->mixMusic = Mix_LoadMUS(path);
		if (music->mixMusic == nullptr)
		{
			exceptionManager->PushException(Ptr<Exception>::New(((String)"无法加载音频文件：" + filePath)));
			return nullptr;
		}

		this->musicList.Add(music);
		return music;
	}

	void Audio::PlayMusic(Ptr<Music> music)
	{
		PlayMusic(music, false);
	}

	void Audio::PlayMusic(Ptr<Music> music, bool looped)
	{
		if (currentMusic == music && Mix_PlayingMusic() == 1)
			return;

		if (currentMusic != nullptr && currentMusic != music)
		{
			currentMusic->Stop();
		}
		currentMusic = music;

		if (looped)
		{
			Mix_PlayMusic(music->mixMusic, -1);
		}
		else
		{
			Mix_PlayMusic(music->mixMusic, 1);
		}
		float volume = music->volume * this->musicVolume / 10000.0f;
		Mix_VolumeMusic(volume * MIX_MAX_VOLUME);

		Mix_HookMusicFinished([]() {
			auto audio = Audio::GetInstance();
			if (audio)
				audio->OnMusicFinished();
		});
	}

	void Audio::OnMusicFinished()
	{
		this->currentMusic = nullptr;
	}

	void Audio::StopMusic(Ptr<Music> music)
	{
		if (currentMusic != music)
			return;

		if (music != nullptr)
		{
			Mix_HaltMusic();
		}
		currentMusic = nullptr;
	}

	void Audio::DestroyMusic(Ptr<Music> music)
	{
		if (music == currentMusic)
		{
			StopMusic(music);
		}
		this->musicList.Remove(music);
	}

	void Audio::Update()
	{
		auto iter = soundList.GetIterator();
		while (iter->HasNext())
		{
			auto sound = iter->Next();
			sound->submitted = false;
		}
	}
}