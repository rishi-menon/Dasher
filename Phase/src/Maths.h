#pragma once
#include <glm.hpp>
#include <gtx/compatibility.hpp>

namespace Math {
	inline double Lerp(double a, double b, double percent)
	{
		return a + (b - a) * percent;
	}

	inline double ClampLeft(double min, double value)
	{
		if (value < min)		return min;
		else					return value;
	}

	inline double ClampRight(double max, double value)
	{
		if (value > max)		return max;
		else					return value;
	}

	inline double Clamp(double min, double max, double value)
	{
		if (value < min)		return min;
		else if (value > max)	return max;
		else					return value;
	}
	inline double Clamp01(double value)
	{
		if (value < 0.0)		return 0.0;
		else if (value > 1.0)	return 1.0;
		else					return value;
	}
	inline double GetPercent(double min, double max, double value)
	{
		return (value - min) / (max - min);
	}
}

namespace Random
{
	extern void Init();
	extern double Rand();
	extern double Rand(double min, double max);
}