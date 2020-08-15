#pragma once
#include <glm.hpp>
#include <gtx/compatibility.hpp>

namespace Math {
	inline double Lerp(double a, double b, double percent)
	{
		const double val = a + (b - a) * percent;
		return val;
	}
	inline double Clamp(double min, double max, double value)
	{
		if (value < min)	return min;
		if (value > max)	return max;
		return value;
	}
}

namespace Random
{
	extern void Init();
	extern double Rand();
	extern double Rand(double min, double max);
}