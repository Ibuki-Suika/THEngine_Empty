#ifndef TH_ASYNC_LOADER_H
#define TH_ASYNC_LOADER_H

#include <Common\THCommon.h>

namespace THEngine
{
	class AsyncInfo;

	class AsyncLoader : public Object
	{
	private:
		using Callback = std::function<void()>;

	private:
		AsyncLoader() = default;
		virtual ~AsyncLoader() = default;

		static void ThreadFuncLoad(AsyncInfo* info, EngineObject* object);

		static void Load(EngineObject* object, const Callback& onCompleted, AsyncInfo* outputInfo);

	public:
		static void Load(EngineObject* object);

		static void Load(EngineObject* object, const Callback& onCompleted);

		static AsyncInfo* LoadWithInfo(EngineObject* object);

		static AsyncInfo* LoadWithInfo(EngineObject* object, const Callback& onCompleted);
	};
}

#endif
