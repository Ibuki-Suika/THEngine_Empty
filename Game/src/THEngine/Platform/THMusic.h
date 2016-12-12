#ifndef THMUSIC_H
#define THMUSIC_H

#include "../Common/THCommon.h"
#include "External\CWaveFile.h"
#include <xaudio2.h>

namespace THEngine
{
	class Music : public Object
	{
	protected:
		IXAudio2SourceVoice* sourceVoice;
		XAUDIO2_BUFFER buffer;

		CWaveFile wave;

		String path;

	public:
		Music();
		virtual ~Music();
	};
}

#endif