#ifndef THDEFINE_H
#define THDEFINE_H

#include <assert.h>
#include <limits>  

#ifndef ASSERT
#define ASSERT assert
#endif

//Fix VS 2015 link errors
#if _MSC_VER >= 1900       
//http://stackoverflow.com/questions/30450042/unresolved-external-symbol-imp-iob-func-referenced-in-function-openssldie
FILE _iob[] = { *stdin, *stdout, *stderr }; 
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }

//http://stackoverflow.com/questions/31053670/unresolved-external-symbol-vsnprintf-in-dxerr-lib
#pragma comment(lib,"legacy_stdio_definitions.lib")
#endif

#define TH_SAFE_RELEASE(object)  \
	if(object) { object->Release(); object = NULL; }

#define TH_SAFE_DELETE(object)  \
	if(object) { delete object; object = NULL; }

#define TH_INSTANCEOF(instance,classname) (typeid(instance) == typeid(classname))

#define  TH_MAX_FLOAT        FLT_MAX
#define  TH_MAX_Z            65535

#endif