#include "THCommon.h"

using namespace THEngine;

//Fix VS 2015 link errors
#if _MSC_VER >= 1900       
//http://stackoverflow.com/questions/30450042/unresolved-external-symbol-imp-iob-func-referenced-in-function-openssldie
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
#endif

namespace THEngine
{
	void THMessageBox(String message)
	{
		MessageBox(NULL, message.GetBuffer(), NULL, MB_OK);
	}
}


