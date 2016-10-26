namespace sh
{
	namespace math
	{
		template<typename T>
		inline const T& min_(const T& a, const T& b)
		{
			return a < b ? a : b;
		}

		template<typename T>
		inline const T& max_(const T& a, const T& b)
		{
			return a < b ? b : a;
		}

		template<typename T>
		inline const T& min_(const T& a, const T& b, const T& c)
		{
			return min_(a, min_(b, c));
		}

		template<typename T>
		inline const T& max_(const T& a, const T& b, const T& c)
		{
			return max_(a, max_(b, c));
		}

		template<typename T>
		inline T clamp(const T& x, const T& _min, const T& _max)
		{
			return min_(max_(x, _min), _max);
		}

		//////////////////////////////////////////////////////////////////////////
		// vec2

		template<typename T>
		inline vec2<T> min_(const vec2<T>& a, const vec2<T>& b)
		{
			return vec2<T>(min_(a.x, b.x), min_(a.y, b.y));
		}
		template<typename T>
		inline vec2<T> max_(const vec2<T>& a, const vec2<T>& b)
		{
			return vec2<T>(max_(a.x, b.x), max_(a.y, b.y));
		}

		//////////////////////////////////////////////////////////////////////////
		// vec3

		template<typename T>
		inline vec3<T> min_(const vec3<T>& a, const vec3<T>& b)
		{
			return vec3<T>(min_(a.x, b.x), min_(a.y, b.y), min_(a.z, b.z));
		}
		template<typename T>
		inline vec3<T> max_(const vec3<T>& a, const vec3<T>& b)
		{
			return vec3<T>(max_(a.x, b.x), max_(a.y, b.y), max_(a.z, b.z));
		}

		//////////////////////////////////////////////////////////////////////////
		// vec4

		template<typename T>
		inline vec4<T> min_(const vec4<T>& a, const vec4<T>& b)
		{
			return vec4<T>(min_(a.x, b.x), min_(a.y, b.y), min_(a.z, b.z), min_(a.w, b.w));
		}
		template<typename T>
		inline vec4<T> max_(const vec4<T>& a, const vec4<T>& b)
		{
			return vec4<T>(max_(a.x, b.x), max_(a.y, b.y), max_(a.z, b.z), max_(a.w, b.w));
		}

		template<typename T>
		T floor(const T& x)
		{
			return std::floor(x);
		}

		template<typename T>
		T ceil(const T& x)
		{
			return std::ceil(x);
		}
		template<typename T>
		T fract(const T& x)
		{
			return x - floor(x);
		}

		//rounds to the closest integer
		inline float round(float x)
		{
			return floor(x + 0.5f);
		}

		//rounds to the closest integer
		inline double round(double x)
		{
			return floor(x + 0.5);
		}

		//rounds to the closest integer
		template<typename T>
		inline T round(const T& x)
		{
			return x; //for non-float types, nothing
		}
	}
}