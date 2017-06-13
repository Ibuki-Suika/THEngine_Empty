#include "THSound.h"

namespace THEngine
{
	Sound::Sound()
	{

	}

	Sound::~Sound()
	{
		if (sourceVoice)
		{
			sourceVoice->DestroyVoice();
			sourceVoice = nullptr;
		}

		TH_SAFE_DELETE(callback);

		if (buffer.pAudioData)
		{
			delete[] buffer.pAudioData;
		}
	}

	void Sound::Play()
	{
		if (submitted == false)
		{
			XAUDIO2_VOICE_STATE state;
			sourceVoice->GetState(&state);
			if (state.BuffersQueued > 0)
			{
				sourceVoice->Stop();
				sourceVoice->FlushSourceBuffers();
			}
			sourceVoice->SubmitSourceBuffer(&buffer);
			sourceVoice->Start(0);
			submitted = true;
		}
	}
}

