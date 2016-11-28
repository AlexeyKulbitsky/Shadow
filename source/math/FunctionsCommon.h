#ifndef SHADOW_FUNCTIONS_COMMON_INCLUDE
#define SHADOW_FUNCTIONS_COMMON_INCLUDE

#include <cmath>

namespace sh
{
	namespace math
	{
		template<typename T>
		T Sqrt(T value)
		{
			std::sqrt(value);
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
	}
}

#endif
