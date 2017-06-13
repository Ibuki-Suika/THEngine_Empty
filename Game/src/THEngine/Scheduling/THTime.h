#ifndef TH_TIME_H
#define TH_TIME_H

namespace THEngine
{
	class Time
	{
	private:
		long long microsecond = 0;

		static const long long MICROSECOND_PER_MILLISECOND = 1000;
		static const long long MICROSECOND_PER_SECOND = MICROSECOND_PER_MILLISECOND * 1000;
		static const long long MICROSECOND_PER_MINUTE = MICROSECOND_PER_SECOND * 60;
		static const long long MICROSECOND_PER_HOUR = MICROSECOND_PER_MINUTE * 60;

	public:
		Time() = default;
		Time(long long microsecond) : microsecond(microsecond) {}

		void Set(int hour, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);

		inline void FromMicrosecond(long long microsecond)
		{
			this->microsecond = microsecond;
		}

		inline long long ToMicroSecond() const { return this->microsecond; }

		Time operator- (const Time& other) const { return Time(this->microsecond - other.microsecond); }
	};
}

#endif