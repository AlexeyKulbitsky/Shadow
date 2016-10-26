#pragma once
namespace sh
{

	namespace math
	{
		template<class T>
		T inverse(const T& v);

		template<class T>
		T transposed(const T& v);

		template<class T>
		T normalized(const T& v);

		template<class T>
		T normalizedFast(const T& v);

		template<class T>
		typename T::value_t dot(const T& v1, const T& v2);

		template<class T>
		T cross(const T& v1, const T& v2);

		// special case for 2d cross
		template<class T>
		T cross(const vec2<T>& v1, const vec2<T>& v2);

		// special case for 4d cross
		template<class T>
		vec4<T> cross(const vec4<T>& x1, const vec4<T>& x2, const vec4<T>& x3);

		template<class T>
		T square(const T& v);

		template<class T>
		T distance(const T& v1, const T& v2);

		template<class T>
		T distanceSq(const T& v1, const T& v2);


	}

}
