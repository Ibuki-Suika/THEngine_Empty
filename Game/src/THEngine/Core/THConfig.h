#ifndef TH_CONFIG_H
#define TH_CONFIG_H

#include <Common\THCommon.h>

namespace THEngine
{
	struct Config
	{
		int width = 640;
		int height = 480;
		bool fullScreen = false;
		String title = "THEngine";
		bool useMultiSample = true;
		bool useVSync = true;
		bool useShadow = true;
		bool useLighting = true;
		int fps = 60;         //this option is valid only if useVsync == false
	};
}

#endif
