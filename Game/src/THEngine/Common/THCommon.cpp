#include "THCommon.h"

using namespace THEngine;

namespace THEngine
{
	void THMessageBox(String message)
	{
		MessageBox(NULL, message.GetBuffer(), NULL, MB_OK);
	}
}


