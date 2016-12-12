#ifndef THDEFINE_H
#define THDEFINE_H

#include <assert.h>
#include <limits>  

#ifndef ASSERT
#define ASSERT assert
#endif

#define TH_SAFE_RELEASE(object)  \
	if(object) { object->Release(); object = NULL; }

#define TH_SAFE_DELETE(object)  \
	if(object) { delete object; object = NULL; }

#define TH_INSTANCEOF(instance,classname) (typeid(instance) == typeid(classname))

#define  TH_MAX_FLOAT        FLT_MAX
#define  TH_MAX_Z            65535

#endif