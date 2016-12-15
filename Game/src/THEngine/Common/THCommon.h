#ifndef THCOMMON_H
#define THCOMMON_H

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

//Fix VS 2015 link errors
#if _MSC_VER >= 1900
//http://stackoverflow.com/questions/31053670/unresolved-external-symbol-vsnprintf-in-dxerr-lib
#pragma comment(lib,"legacy_stdio_definitions.lib")
#endif

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION   0x0800
#endif

#include "THObject.h"
#include "THEngineObject.h"
#include "THDefine.h"
#include "THString.h"
#include "THExceptionManager.h"
#include "THLogger.h"
#include "THList.h"
#include "THMap.h"

namespace THEngine
{
	void THMessageBox(String message);

	inline void THLog(String message)
	{
		Logger::GetInstance()->WriteLine(message);
	}
}

#endif