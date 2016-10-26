#pragma once

namespace sh
{

	namespace math
	{
		template<class T>
		T sqrt(const T& v);

		template<class T>
		T fastSqrt(const T& v);

		template<class T>
		T invSqrt(const T& v);

		template<class T>
		T invSqrtFast(const T& v);

		// specialization for 64 bit integer because it is not available in stl
		int64_t abs(const int64_t& a);

		template<class T>
		T abs(const T& v);

		template<class T>
		T sgn(const T& v);

		template<class T>
		T pow(const T& a, const T& b);

		template<class T>
		T nearestMultiple(const T& a, const T& b);

		template<class T>
		T nextMultiple(const T& a, const T& b);

	}

}
