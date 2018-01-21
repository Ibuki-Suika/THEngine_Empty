#include "THCommon.h"

using namespace THEngine;

//Fix VS 2015 link errors
//http://stackoverflow.com/questions/30450042/unresolved-external-symbol-imp-iob-func-referenced-in-function-openssldie
#if _MSC_VER >= 1900       
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


