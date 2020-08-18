#include "Maths.h"

#include <random>
#include <chrono>

namespace Math {

}

namespace Random
{
	static std::default_random_engine generator;
	static std::uniform_real_distribution<double> distReal(0.0f, 1.0f);

	extern void Init()
	{
		//Get system time/time since epoch for the initial seed value
		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
		long long value = std::chrono::time_point_cast <std::chrono::milliseconds> (timeNow).time_since_epoch().count();

		generator.seed(static_cast<unsigned int>(value));
	}
	extern double Rand()
	{
		return distReal(generator);
	}
	extern double Rand(double min, double max)
	{
		return Math::Lerp(min, max, distReal(generator));
	}
}