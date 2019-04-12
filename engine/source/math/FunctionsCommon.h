#ifndef SHADOW_FUNCTIONS_COMMON_INCLUDE
#define SHADOW_FUNCTIONS_COMMON_INCLUDE

#include "math/Constants.h"

#include <cmath>
#include <algorithm>

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
		T Asin(const T& value)
		{
			return std::asin(value);
		}

		template<typename T>
		T Acos(const T& value)
		{
			return std::acos(value);
		}

		template<typename T>
		T Atan2(const T& y, const T& x)
		{
			return std::atan2(y, x);
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

		template<typename T>
		T Abs(const T& value)
		{
			return std::abs(value);
		}

		template<typename T>
		T Min(const T& a, const T& b)
		{
			return std::min(a, b);
		}

		template<typename T>
		T Max(const T& a, const T& b)
		{
			return std::max(a, b);
		}

		template<typename T>
		T Clamp(const T& value, const T& min, const T& max)
		{
			return Min(Max(value, min), max);
		}
	}
}

#endif
