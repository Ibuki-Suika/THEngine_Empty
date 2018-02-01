#ifndef THAUDIO_H
#define THAUDIO_H

#include <Common\THCommon.h>
#include <xaudio2.h>
#include "THSound.h"
#include "THMusic.h"

namespace THEngine
{
	class Audio : public Object, public Singleton<Audio>
	{
		friend class Singleton<Audio>;

	private:
		IXAudio2* xaudio = nullptr;
		IXAudio2MasteringVoice* masterVoice = nullptr;

		ArrayList<Ptr<Sound>> soundList;
		ArrayList<Ptr<Music>> musicList;

		int volume = 20;    //Range from 0 to 100
		int musicVolume = 50;

		Ptr<Music> currentMusic;

	private:
		Audio() = default;
		Ptr<Sound> LoadWav(const String& filePath);

	public:
		virtual ~Audio();

		bool Init();

		Ptr<Sound> CreateSound(const String& filePath);
		Ptr<Music> CreateMusic(const String& filePath);

		inline void DestroySound(Ptr<Sound> sound)
		{
			soundList.Remove(sound);
		}
		void DestroyMusic(Ptr<Music> music);

		inline void SetVolume(int volume) { this->volume = volume; }

		void PlayMusic(Ptr<Music> music);
		void PlayMusic(Ptr<Music> music, bool looped);
		void StopMusic(Ptr<Music> music);

		inline Ptr<Music> GetCurrentMusic() const { return this->currentMusic; }

		void Update();

		void OnMusicFinished();
	};
}

#endif