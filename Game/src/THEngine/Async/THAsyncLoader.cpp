#include "THAsyncLoader.h"
#include "THAsyncInfo.h"
#include <thread>

namespace THEngine
{
	void AsyncLoader::Load(Ptr<EngineObject> object)
	{
		Load(object, []() {});
	}

	void AsyncLoader::Load(Ptr<EngineObject> object, const Callback& onCompleted)
	{
		_Load(object, onCompleted);
	}

	Ptr<AsyncInfo> AsyncLoader::_Load(Ptr<EngineObject> object, const Callback& onCompleted)
	{
		Ptr<AsyncInfo> info = Ptr<AsyncInfo>::New();
		info->onFinished = onCompleted;

		std::thread threadLoad(&AsyncLoader::ThreadFuncLoad, info, object);
		threadLoad.detach();

		return info;
	}

	Ptr<AsyncInfo> AsyncLoader::LoadWithInfo(Ptr<EngineObject> object)
	{
		return LoadWithInfo(object, []() {});
	}

	Ptr<AsyncInfo> AsyncLoader::LoadWithInfo(Ptr<EngineObject> object, const Callback& onCompleted)
	{
		Ptr<AsyncInfo> info = _Load(object, onCompleted);
		return info;
	}

	void AsyncLoader::ThreadFuncLoad(Ptr<AsyncInfo> info, Ptr<EngineObject> object)
	{
		object->OnLoad(info);
		info->onFinished();
	}
}