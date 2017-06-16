#ifndef THMUSIC_H
#define THMUSIC_H

#include <Common\THCommon.h>
#include "External\CWaveFile.h"
#include <External\SDL2\SDL.h>
#include <External\SDL2\SDL_mixer.h>
#include <xaudio2.h>

namespace THEngine
{
	class Music : public Object
	{
	protected:
		Mix_Music* mixMusic = nullptr;
		String path;
		int volume = 100;

	public:
		Music();
		virtual ~Music();

		void Play();
		void Pause();
		void Resume();
		void Stop();
		void FadeOut(int millisecond);

		friend class Audio;
	};
}

#endif