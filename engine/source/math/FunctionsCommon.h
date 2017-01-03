#ifndef SHADOW_FUNCTIONS_COMMON_INCLUDE
#define SHADOW_FUNCTIONS_COMMON_INCLUDE

#include <cmath>
#include "Constants.h"

namespace sh
{
	namespace math
	{
		template<typename T>
		T Sqrt(T value)
		{
			return std::sqrt(value);
		}

		template<typename T>
		T Sin(T value)
		{
			return std::sin(value);
		}

		template<typename T>
		T Cos(T value)
		{
			return std::cos(value);
		}

		template<typename T>
		T Tan(T value)
		{
			return std::tan(value);
		}

		template<typename T>
		T Radians(const T& degrees)
		{
			return (degrees / 180.0f) * k_pi;
		}

		template<typename T>
		T Degrees(const T& radians)
		{
			return (radians / k_pi) * 180.0f;
		}
	}
}

#endif
