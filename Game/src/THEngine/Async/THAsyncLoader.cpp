#include "THAsyncLoader.h"
#include "THAsyncInfo.h"
#include <thread>

namespace THEngine
{
	void AsyncLoader::Load(EngineObject* object)
	{
		Load(object, []() {}, nullptr);
	}

	void AsyncLoader::Load(EngineObject* object, const Callback& onCompleted)
	{
		Load(object, onCompleted, nullptr);
	}

	void AsyncLoader::Load(EngineObject* object, const Callback& onCompleted, AsyncInfo* outputInfo)
	{
		AsyncInfo* info;

		if (outputInfo)
		{
			outputInfo->Retain();
			info = outputInfo;
		}
		else
		{
			info = new AsyncInfo();
		}
		info->onFinished = onCompleted;

		std::thread threadLoad(&AsyncLoader::ThreadFuncLoad, info, object);
		threadLoad.detach();
	}

	AsyncInfo* AsyncLoader::LoadWithInfo(EngineObject* object)
	{
		return LoadWithInfo(object, []() {});
	}

	AsyncInfo* AsyncLoader::LoadWithInfo(EngineObject* object, const Callback& onCompleted)
	{
		AsyncInfo* info = new AsyncInfo();
		Load(object, onCompleted, info);
		return info;
	}

	void AsyncLoader::ThreadFuncLoad(AsyncInfo* info, EngineObject* object)
	{
		info->Retain();
		object->OnLoad(info);

		info->onFinished();

		TH_SAFE_RELEASE(info);
	}
}