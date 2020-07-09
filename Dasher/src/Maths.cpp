#include "Maths.h"

namespace Math {
	double Lerp(double a, double b, double percent)
	{
		const double val = a + (b - a) * percent;
		return val;
	}
	double Clamp(double min, double max, double value)
	{
		if (value < min)	return min;
		if (value > max)	return max;
		return value;
	}
}