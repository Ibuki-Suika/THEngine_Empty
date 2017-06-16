#ifndef THAUDIO_H
#define THAUDIO_H

#include <Common\THCommon.h>
#include <xaudio2.h>
#include "THSound.h"
#include "THMusic.h"

namespace THEngine
{
	class Audio : public Object
	{
	private:
		static Audio* instance;

		IXAudio2* xaudio = nullptr;
		IXAudio2MasteringVoice* masterVoice = nullptr;

		ArrayList<Sound*> soundList;
		ArrayList<Music*> musicList;

		int volume = 20;    //Range from 0 to 100
		int musicVolume = 50;

		Music* currentMusic = nullptr;
	private:
		Audio();
		bool Init();

		Sound* LoadWav(const String& filePath);

	public:
		virtual ~Audio();

		static Audio* GetInstance();

		Sound* CreateSound(const String& filePath);
		Music* CreateMusic(const String& filePath);

		inline void DestroySound(Sound* sound)
		{
			soundList.Remove(sound);
		}
		void DestroyMusic(Music* music);

		inline void SetVolume(int volume) { this->volume = volume; }

		void PlayMusic(Music* music);
		void PlayMusic(Music* music, bool looped);
		void StopMusic(Music* music);

		inline Music* GetCurrentMusic() const { return this->currentMusic; }

		void Update();

		void OnMusicFinished();
	};
}

#endif