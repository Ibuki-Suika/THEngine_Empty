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

		static void ThreadFuncLoad(Ptr<AsyncInfo> info, Ptr<EngineObject> object);

		static Ptr<AsyncInfo> _Load(Ptr<EngineObject> object, const Callback& onCompleted);
	public:
		static void Load(Ptr<EngineObject> object);

		static void Load(Ptr<EngineObject> object, const Callback& onCompleted);

		static Ptr<AsyncInfo> LoadWithInfo(Ptr<EngineObject> object);

		static Ptr<AsyncInfo> LoadWithInfo(Ptr<EngineObject> object, const Callback& onCompleted);
	};
}

#endif
