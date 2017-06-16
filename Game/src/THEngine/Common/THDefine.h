#ifndef THDEFINE_H
#define THDEFINE_H

#include <assert.h>
#include <limits>
#include <mutex>

#ifndef ASSERT
#define ASSERT assert
#endif

#define TH_SAFE_RELEASE(object)  \
	if(object) { object->Release(); object = NULL; }

#define TH_SAFE_DELETE(object)  \
	if(object) { delete object; object = NULL; }

#define TH_SET(object, value) \
	if(object != value)       \
	{                         \
		TH_SAFE_RELEASE(object); \
		object = value; \
		if(object) \
			object->Retain(); \
	}

#define TH_LOCK(mutex)  \
    for(auto __s__ = std::make_pair(  \
            std::unique_lock<std::remove_reference<decltype(mutex)>::type>(mutex), \
            false  \
        ); __s__.second == false; __s__.second = true)

#define TH_INSTANCEOF(instance,classname) (typeid(instance) == typeid(classname))

#define  TH_MAX_FLOAT        FLT_MAX
#define  TH_MAX_Z            65535

#endif