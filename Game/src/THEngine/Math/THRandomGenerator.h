#ifndef TH_RANDOM_GENERATOR_H
#define TH_RANDOM_GENERATOR_H

#include <Common\THCommon.h>
#include <random>

namespace THEngine
{
	class RandomGenerator : public Object
	{
	private:
		int seed = 1;
		std::default_random_engine generator;
		std::uniform_int_distribution<> dist;

	public:
		RandomGenerator(int seed);
		virtual ~RandomGenerator() = default;

		//return a int in [a, b]
		int Next(int a, int b);

		void Reset();
		void Reset(int seed);

		inline int GetSeed() const { return this->seed; }
	};
}

#endif
