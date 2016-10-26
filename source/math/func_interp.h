#pragma once
namespace sh
{


	namespace math
	{
		//////////////////////////////////////////////////////////////////////////
		// mixing values based on a weight.
		// lerp(a, b, t):
		// if t == 0
		//		return a
		// if (t == 1)
		//		return b
		// else 
		//		return a*(t-1) + b*t

		int8_t lerp(int8_t a, int8_t b, float t);
		uint8_t lerp(uint8_t a, uint8_t b, float t);

		int16_t lerp(int16_t a, int16_t b, float t);
		uint16_t lerp(uint16_t a, uint16_t b, float t);

		int32_t lerp(int32_t a, int32_t b, float t);
		uint32_t lerp(uint32_t a, uint32_t b, float t);

		//Do not use s64/u64 with lerp.
		//There is no safe and accurate method of mixing them. Avoid or find a different way
	// 	s64 lerp(s64 a, s64 b, float t);
	// 	u64 lerp(u64 a, u64 b, float t);

		float lerp(float a, float b, float t);
		double lerp(double a, double b, float t);

		template<class T>
		T lerp(const T& a, const T& b, float t);

		//does simple linear interpolation (as opposed to spherical linear interpolation) of quaternions
		template<typename T>
		quat<T> nlerp(const quat<T>& a, const quat<T>& b, float t);

		//equivalent to lerp
		template<typename T>
		quat<T> slerp(const quat<T>& a, const quat<T>& b, float t);
	}

}
