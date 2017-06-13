#include "THMusic.h"
#include "THAudio.h"

#pragma comment(lib, "SDL2/SDL2.lib")
#pragma comment(lib, "SDL2/SDL2_mixer.lib")

namespace THEngine
{
	Music::Music()
	{
		this->volume = 100;
	}

	Music::~Music()
	{
		Mix_FreeMusic(this->mixMusic);
	}

	void Music::Play()
	{
		Audio::GetInstance()->PlayMusic(this);
	}

	void Music::Pause()
	{
		if (Audio::GetInstance()->GetCurrentMusic() != this)
			return;

		if (Mix_PausedMusic() == 0)
			Mix_PauseMusic();
	}

	void Music::Resume()
	{
		if (Audio::GetInstance()->GetCurrentMusic() != this)
			return;

		if (Mix_PausedMusic() == 1)
			Mix_ResumeMusic();
	}

	void Music::Stop()
	{
		Audio::GetInstance()->StopMusic(this);
	}

	void Music::FadeOut(int millisecond)
	{
		if (Audio::GetInstance()->GetCurrentMusic() != this)
			return;

		Mix_FadeOutMusic(millisecond);
	}
}