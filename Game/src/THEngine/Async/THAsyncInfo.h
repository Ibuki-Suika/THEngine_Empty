#ifndef TH_ASYNC_INFO_H
#define TH_ASYNC_INFO_H

#include <Common\THCommon.h>
#include <mutex>

namespace THEngine
{
	class AsyncInfo : public Object
	{
	private:
		using Callback = std::function<void()>;

	private:
		int progress = 0;
		int maxProgress = 100;
		bool finished = false;
		Callback onFinished;
		std::mutex mutex;

	public:
		AsyncInfo() = default;
		virtual ~AsyncInfo() = default;

		inline bool IsFinished() const { return this->finished; }

		inline int GetProgress() const { return this->progress; }

		friend class AsyncLoader;
	};
}

#endif
