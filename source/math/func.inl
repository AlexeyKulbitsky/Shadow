namespace sh
{
	namespace math
	{

		//////////////////////////////////////////////////////////////////////////

		template<class T>
		inline T abs(const T& v)
		{
			return std::abs(v);
		}

		template<>
		inline float abs(const float& v)
		{
			uint32_t a = (*(int32_t *)&v) & 0x7fffffff;
			float f = *(float*)&a;
			return f;
		}

		inline int64_t abs(const int64_t& a)
		{
			return a < 0 ? -a : a;
		}

		template<class T>
		inline T sgn(const T& v)
		{
			return v < T(0) ? T(-1) : v == T(0) ? T(0) : T(1);
		}

		template<class T>
		inline vec2<T> abs(const vec2<T>& v)
		{
			return vec2<T>(abs(v.x), abs(v.y));
		}

		template<class T>
		inline vec2<T> sgn(const vec2<T>& v)
		{
			return vec2<T>(sgn(v.x), sgn(v.y));
		}

		template<class T>
		inline vec3<T> abs(const vec3<T>& v)
		{
			return vec3<T>(abs(v.x), abs(v.y), abs(v.z));
		}

		template<class T>
		inline vec3<T> sgn(const vec3<T>& v)
		{
			return vec3<T>(sgn(v.x), sgn(v.y), sgn(v.z));
		}

		template<class T>
		inline vec4<T> abs(const vec4<T>& v)
		{
			return vec4<T>(abs(v.x), abs(v.y), abs(v.z), abs(v.w));
		}

		template<class T>
		inline vec4<T> sgn(const vec4<T>& v)
		{
			return vec4<T>(sgn(v.x), sgn(v.y), sgn(v.z), sgn(v.w));
		}

		//////////////////////////////////////////////////////////////////////////

		template<class T>
		inline T sqrt(const T& v)
		{
			assert(v >= T(0));
			return static_cast<T>(std::sqrt(v));
		}

		template<class T>
		inline vec2<T> sqrt(const vec2<T>& v)
		{
			return vec2<T>(sqrt(v.x), sqrt(v.y));
		}

		template<class T>
		inline vec3<T> sqrt(const vec3<T>& v)
		{
			return vec3<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z));
		}

		template<class T>
		inline vec4<T> sqrt(const vec4<T>& v)
		{
			return vec4<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z), sqrt(v.w));
		}

		//////////////////////////////////////////////////////////////////////////

		inline float fastSqrt(float f)
		{
			assert(f == f  &&  f >= 0);
			long tmp = *(long *)&f;
			tmp -= 127L << 23; /* Remove IEEE bias from exponent (-2^23) */
							   /* tmp is now an appoximation to logbase2(val) */
			tmp = tmp >> 1; /* divide by 2 */
			tmp += 127L << 23; /* restore the IEEE bias from the exponent (+2^23) */
			return *(float *)&tmp;
		}

		//////////////////////////////////////////////////////////////////////////

		template<class T>
		inline T invSqrt(const T& v)
		{
			assert(!isZero(v));
			return 1 / sqrt(v);
		}

		template<class T>
		inline vec2<T> invSqrt(const vec2<T>& v)
		{
			return vec2<T>(invSqrt(v.x), invSqrt(v.y));
		}

		template<class T>
		inline vec3<T> invSqrt(const vec3<T>& v)
		{
			return vec3<T>(invSqrt(v.x), invSqrt(v.y), invSqrt(v.z));
		}

		template<class T>
		inline vec4<T> invSqrt(const vec4<T>& v)
		{
			return vec4<T>(invSqrt(v.x), invSqrt(v.y), invSqrt(v.z), invSqrt(v.w));
		}

		//////////////////////////////////////////////////////////////////////////


		template<>
		inline float invSqrtFast(const float& v)
		{
			//assert(!isZero(v));
			float xhalf = 0.5f*v;
			int i = *(int*)&v;
			i = 0x5f3759df - (i >> 1);
			float r = *(float*)&i;
			r = r*(1.5f - xhalf*r*r);
			return r;
		}


		template<class T>
		inline T invSqrtFast(const T& v)
		{
			assert(!isZero(v));
			return 1 / sqrt(v);
		}

		template<class T>
		inline vec2<T> invSqrtFast(const vec2<T>& v)
		{
			return vec2<T>(invSqrtFast(v.x), invSqrtFast(v.y));
		}

		template<class T>
		inline vec3<T> invSqrtFast(const vec3<T>& v)
		{
			return vec3<T>(invSqrtFast(v.x), invSqrtFast(v.y), invSqrtFast(v.z));
		}

		template<class T>
		inline vec4<T> invSqrtFast(const vec4<T>& v)
		{
			return vec4<T>(invSqrtFast(v.x), invSqrtFast(v.y), invSqrtFast(v.z), invSqrtFast(v.w));
		}

		//////////////////////////////////////////////////////////////////////////

		template<class T>
		inline T pow(const T& a, const T& b)
		{
			return std::pow(a, b);
		}

		template<class T>
		inline vec2<T> pow(const vec2<T>& a, const vec2<T>& b)
		{
			return vec2<T>(pow(a.x, b.x), pow(a.y, b.y));
		}

		template<class T>
		inline vec3<T> pow(const vec3<T>& a, const vec3<T>& b)
		{
			return vec3<T>(pow(a.x, b.x), pow(a.y, b.y), pow(a.z, b.z));
		}

		template<class T>
		inline vec4<T> pow(const vec4<T>& a, const vec4<T>& b)
		{
			return vec4<T>(pow(a.x, b.x), pow(a.y, b.y), pow(a.z, b.z), pow(a.w, b.w));
		}


		//////////////////////////////////////////////////////////////////////////

		inline float nearestMultiple(const float a, const float b)
		{
			float tmp = fmodf(a, b);
			float left = a - tmp;
			float right = left + b;
			if (a - left < right - a)
			{
				return left;
			}
			return right;
		}

		//////////////////////////////////////////////////////////////////////////

		template<class T>
		inline T nearestMultiple(const T& a, const T& b)
		{
			T tmp = a % b;
			T left = a - tmp;
			T right = left + b;
			if (a - left < right - a)
			{
				return left;
			}
			return right;
		}

		//////////////////////////////////////////////////////////////////////////

		template<class T>
		inline T nextMultiple(const T& a, const T& b)
		{
			T tmp = nearestMultiple(a, b);
			if (tmp > a)
			{
				return tmp;
			}
			return tmp + b;
		}

	}
}