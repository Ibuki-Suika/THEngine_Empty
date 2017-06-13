#ifndef THSOUND_H
#define THSOUND_H

#include <Common\THCommon.h>
#include <xaudio2.h>

namespace THEngine
{
	class Sound : public Object
	{
	public:
		class SoundCallback : public IXAudio2VoiceCallback
		{
		private:
			Sound* sound;

		public:
			SoundCallback(Sound* sound) { this->sound = sound; }
			~SoundCallback() {}

			//Called when the voice has just finished playing a contiguous audio stream.
			virtual void _stdcall OnStreamEnd() override { sound->Stop(); }

			//Unused methods are stubs
			virtual void _stdcall OnVoiceProcessingPassEnd() override {}
			virtual void _stdcall OnVoiceProcessingPassStart(UINT32 SamplesRequired) override {}
			virtual void _stdcall OnBufferEnd(void * pBufferContext) override {}
			virtual void _stdcall OnBufferStart(void * pBufferContext) override {}
			virtual void _stdcall OnLoopEnd(void * pBufferContext) override {}
			virtual void _stdcall OnVoiceError(void * pBufferContext, HRESULT Error) override {}
		};

	private:
		IXAudio2SourceVoice* sourceVoice = nullptr;
		XAUDIO2_BUFFER buffer;
		SoundCallback* callback = nullptr;
		bool submitted = false;

	public:
		Sound();
		virtual ~Sound();

		inline void SetVolume(int volume)
		{
			sourceVoice->SetVolume(volume / 100.0f);
		}

		void Play();

		inline void Stop() { sourceVoice->Stop(); }

		friend class Audio;


	};
}

#endif