#pragma once
#include <glm.hpp>
#include <gtx/compatibility.hpp>

namespace Math {
	extern double Lerp(double a, double b, double percent);
	extern double Clamp(double min, double max, double value);

}