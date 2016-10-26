#pragma once

namespace sh
{
	namespace math
	{
		template<typename T>
		const T& min_(const T& a, const T& b);
		template<typename T>
		const T& max_(const T& a, const T& b);

		template<typename T>
		const T& min_(const T& a, const T& b, const T& c);
		template<typename T>
		const T& max_(const T& a, const T& b, const T& c);

		template<typename T>
		T floor(const T& x);
		template<typename T>
		T ceil(const T& x);
		template<typename T>
		T fract(const T& x);

		//rounds to the closest integer
		template<typename T>
		T round(const T& x);

		float round(float x);
		double round(double x);

		template<typename T>
		T clamp(const T& x, const T& min, const T& max);
	}
}
