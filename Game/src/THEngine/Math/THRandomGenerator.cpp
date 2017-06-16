#include "THRandomGenerator.h"
#include <limits>

namespace THEngine
{
	RandomGenerator::RandomGenerator(int seed)
	{
		this->dist = std::uniform_int_distribution<>(0, RAND_MAX);
		Reset(seed);
	}

	void RandomGenerator::Reset()
	{
		Reset(this->seed);
	}

	void RandomGenerator::Reset(int seed)
	{
		this->seed = seed;
		this->generator = std::default_random_engine(seed);
		this->dist.reset();
	}

	int RandomGenerator::Next(int a, int b)
	{
		int temp = this->dist(generator);
		temp %= b - a + 1;
		return (int)temp + a;
	}
}