#pragma once

#include <limits>
#include <assert.h>
#include <cmath>
#include <float.h>

namespace sh
{


	namespace math
	{
		template <typename T> struct vec2;
		template <typename T> struct vec3;
		template <typename T> struct vec4;
		template <typename T> struct mat2;
		template <typename T> struct mat3;
		template <typename T> struct mat4;
		template <typename T> struct quat;
		//////////////////////////////////////////////////////////////////////////
			// equality
			// All functions below use tolerance, even the ones without the tolerance parameter - they use std::limits::epsilon()
			// If you need exact comparison, use ==

		bool equals(float v1, float v2, float tolerance = std::numeric_limits<float>::epsilon());
		bool equals(double v1, double v2, double tolerance = std::numeric_limits<double>::epsilon());
		bool equals(int8_t v1, int8_t v2);
		bool equals(uint8_t v1, uint8_t v2);

		bool equals(int16_t v1, int16_t v2);
		bool equals(uint16_t v1, uint16_t v2);

		bool equals(int32_t v1, int32_t v2);
		bool equals(uint32_t v1, uint32_t v2);

		bool equals(int64_t v1, int64_t v2);
		bool equals(uint64_t v1, uint64_t v2);

		// SGB: it does not compile in VC++2008 SP1
		//	template<typename T>
		//	bool equals(const vec2<T>& v1, const vec2<T>& v2, T tolerance = std::numeric_limits<T>::epsilon());

		bool equals(const vec2<float>& v1, const vec2<float>& v2, float tolerance = std::numeric_limits<float>::epsilon());
		bool equals(const vec3<float>& v1, const vec3<float>& v2, float tolerance = std::numeric_limits<float>::epsilon());
		bool equals(const vec4<float>& v1, const vec4<float>& v2, float tolerance = std::numeric_limits<float>::epsilon());
		bool equals(const mat2<float>& v1, const mat2<float>& v2, float tolerance = std::numeric_limits<float>::epsilon());
		bool equals(const mat3<float>& v1, const mat3<float>& v2, float tolerance = std::numeric_limits<float>::epsilon());
		bool equals(const mat4<float>& v1, const mat4<float>& v2, float tolerance = std::numeric_limits<float>::epsilon());
		bool equals(const quat<float>& v1, const quat<float>& v2, float tolerance = std::numeric_limits<float>::epsilon());

		bool equals(const vec2<double>& v1, const vec2<double>& v2, double tolerance = std::numeric_limits<double>::epsilon());
		bool equals(const vec3<double>& v1, const vec3<double>& v2, double tolerance = std::numeric_limits<double>::epsilon());
		bool equals(const vec4<double>& v1, const vec4<double>& v2, double tolerance = std::numeric_limits<double>::epsilon());
		bool equals(const mat2<double>& v1, const mat2<double>& v2, double tolerance = std::numeric_limits<double>::epsilon());
		bool equals(const mat3<double>& v1, const mat3<double>& v2, double tolerance = std::numeric_limits<double>::epsilon());
		bool equals(const mat4<double>& v1, const mat4<double>& v2, double tolerance = std::numeric_limits<double>::epsilon());
		bool equals(const quat<double>& v1, const quat<double>& v2, double tolerance = std::numeric_limits<double>::epsilon());


		//////////////////////////////////////////////////////////////////////////
		// zero with tolerance

		bool isZero(float v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isZero(double v1, double tolerance = std::numeric_limits<double>::epsilon());

		bool isZero(int8_t v1);
		bool isZero(uint8_t v1);

		bool isZero(int16_t v1);
		bool isZero(uint16_t v1);

		bool isZero(int32_t v1);
		bool isZero(uint32_t v1);

		bool isZero(int64_t v1);
		bool isZero(uint64_t v1);

		bool isZero(const vec2<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isZero(const vec3<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isZero(const vec4<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isZero(const mat2<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isZero(const mat3<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isZero(const mat4<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());

		bool isZero(const vec2<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isZero(const vec3<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isZero(const vec4<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isZero(const mat2<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isZero(const mat3<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isZero(const mat4<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());

		//////////////////////////////////////////////////////////////////////////
		// one with tolerance

		bool isOne(float v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isOne(double v1, double tolerance = std::numeric_limits<double>::epsilon());

		bool isOne(int8_t v1);
		bool isOne(uint8_t v1);

		bool isOne(int16_t v1);
		bool isOne(uint16_t v1);

		bool isOne(int32_t v1);
		bool isOne(uint32_t v1);

		bool isOne(int64_t v1);
		bool isOne(uint64_t v1);

		bool isOne(const vec2<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isOne(const vec3<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isOne(const vec4<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isOne(const mat2<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isOne(const mat3<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isOne(const mat4<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());

		bool isOne(const vec2<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isOne(const vec3<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isOne(const vec4<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isOne(const mat2<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isOne(const mat3<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isOne(const mat4<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());

		//////////////////////////////////////////////////////////////////////////
		// identity with tolerance

		bool isIdentity(const mat2<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isIdentity(const mat3<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isIdentity(const mat4<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());
		bool isIdentity(const quat<float>& v1, float tolerance = std::numeric_limits<float>::epsilon());

		bool isIdentity(const mat2<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isIdentity(const mat3<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isIdentity(const mat4<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());
		bool isIdentity(const quat<double>& v1, double tolerance = std::numeric_limits<double>::epsilon());

		//////////////////////////////////////////////////////////////////////////

		// using isNan instead of isnan for naming coherence
		bool isNan(float);
		bool isNan(double);

	}

}
