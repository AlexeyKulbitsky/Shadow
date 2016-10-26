namespace sh
{
	namespace math
	{


		//////////////////////////////////////////////////////////////////////////
		// scalars

		inline float degrees(float radians)
		{
			return radians * (180.0f / k_pi);
		}
		inline float radians(float degrees)
		{
			return degrees * k_pi / 180.f;
		}
		inline double degrees(double radians)
		{
			return radians * 180.0f / k_pi;
		}
		inline double radians(double degrees)
		{
			return degrees * k_pi / 180.f;
		}
		template<typename T>
		inline T degrees(const T& radians)
		{
			return radians * 180.0f / k_pi;
		}
		template<typename T>
		inline T radians(const T& degrees)
		{
			return degrees * k_pi / 180.f;
		}

		//////////////////////////////////////////////////////////////////////////

		inline float cos(float s)
		{
			return std::cos(s);
		}
		inline float sin(float s)
		{
			return std::sin(s);
		}
		inline float tan(float s)
		{
			return std::tan(s);
		}
		inline float acos(float s)
		{
			return std::acos(s);
		}
		inline float asin(float s)
		{
			return std::asin(s);
		}
		inline float atan(float s)
		{
			return std::atan(s);
		}
		inline float atan2(float y, float x)
		{
			return std::atan2(y, x);
		}
		inline void sincos(float angle, float& s, float& c)
		{
			s = sin(angle);
			c = cos(angle);
		}

		inline double cos(double s)
		{
			return std::cos(s);
		}
		inline double sin(double s)
		{
			return std::sin(s);
		}
		inline double tan(double s)
		{
			return std::tan(s);
		}
		inline double acos(double s)
		{
			return std::acos(s);
		}
		inline double asin(double s)
		{
			return std::asin(s);
		}
		inline double atan(double s)
		{
			return std::atan(s);
		}
		inline double atan2(double y, double x)
		{
			return std::atan2(y, x);
		}
		inline void sincos(double angle, double& s, double& c)
		{
			s = sin(angle);
			c = cos(angle);
		}

		//////////////////////////////////////////////////////////////////////////
		// vec2
		template<typename T>
		inline vec2<T> deg(const vec2<T>& v)
		{
			return vec2<T>(math::deg(v.x), math::deg(v.y));
		}
		template<typename T>
		inline vec2<T> rad(const vec2<T>& v)
		{
			return vec2<T>(math::rad(v.x), math::rad(v.y));
		}

		template<typename T>
		inline vec2<T> cos(const vec2<T>& v)
		{
			return vec2<T>(math::cos(v.x), math::cos(v.y));
		}
		template<typename T>
		inline vec2<T> sin(const vec2<T>& v)
		{
			return vec2<T>(math::sin(v.x), math::sin(v.y));
		}
		template<typename T>
		inline vec2<T> tan(const vec2<T>& v)
		{
			return vec2<T>(math::tan(v.x), math::tan(v.y));
		}
		template<typename T>
		inline vec2<T> acos(const vec2<T>& v)
		{
			return vec2<T>(math::acos(v.x), math::acos(v.y));
		}
		template<typename T>
		inline vec2<T> asin(const vec2<T>& v)
		{
			return vec2<T>(math::asin(v.x), math::asin(v.y));
		}
		template<typename T>
		inline vec2<T> atan(const vec2<T>& v)
		{
			return vec2<T>(math::atan(v.x), math::atan(v.y));
		}
		template<typename T>
		inline vec2<T> atan2(const vec2<T>& y, const vec2<T>& x)
		{
			return vec2<T>(math::atan2(y.x, x.x), math::atan2(y.y, x.y));
		}
		template<typename T>
		inline void sincos(const vec2<T>& angle, vec2<T>& s, vec2<T>& c)
		{
			s = math::sin(angle);
			c = math::cos(angle);
		}

		//////////////////////////////////////////////////////////////////////////
		// vec3
		template<typename T>
		inline vec3<T> deg(const vec3<T>& v)
		{
			return vec3<T>(math::deg(v.x), math::deg(v.y), math::deg(v.z));
		}
		template<typename T>
		inline vec3<T> rad(const vec3<T>& v)
		{
			return vec3<T>(math::rad(v.x), math::rad(v.y), math::rad(v.z));
		}
		template<typename T>
		inline vec3<T> cos(const vec3<T>& v)
		{
			return vec3<T>(math::cos(v.x), math::cos(v.y), math::cos(v.z));
		}
		template<typename T>
		inline vec3<T> sin(const vec3<T>& v)
		{
			return vec3<T>(math::sin(v.x), math::sin(v.y), math::sin(v.z));
		}
		template<typename T>
		inline vec3<T> tan(const vec3<T>& v)
		{
			return vec3<T>(math::tan(v.x), math::tan(v.y), math::tan(v.z));
		}
		template<typename T>
		inline vec3<T> acos(const vec3<T>& v)
		{
			return vec3<T>(math::acos(v.x), math::acos(v.y), math::acos(v.z));
		}
		template<typename T>
		inline vec3<T> asin(const vec3<T>& v)
		{
			return vec3<T>(math::asin(v.x), math::asin(v.y), math::asin(v.z));
		}
		template<typename T>
		inline vec3<T> atan(const vec3<T>& v)
		{
			return vec3<T>(math::atan(v.x), math::atan(v.y), math::atan(v.z));
		}
		template<typename T>
		inline vec3<T> atan2(const vec3<T>& y, const vec3<T>& x)
		{
			return vec3<T>(math::atan2(y.x, x.x), math::atan2(y.y, x.y), math::atan2(y.z, x.z));
		}
		template<typename T>
		inline void sincos(const vec3<T>& angle, vec3<T>& s, vec3<T>& c)
		{
			s = math::sin(angle);
			c = math::cos(angle);
		}

		//////////////////////////////////////////////////////////////////////////
		// vec4
		template<typename T>
		inline vec4<T> deg(const vec4<T>& v)
		{
			return vec4<T>(math::deg(v.x), math::deg(v.y), math::deg(v.z), math::deg(v.w));
		}
		template<typename T>
		inline vec4<T> rad(const vec4<T>& v)
		{
			return vec4<T>(math::rad(v.x), math::rad(v.y), math::rad(v.z), math::rad(v.w));
		}
		template<typename T>
		inline vec4<T> cos(const vec4<T>& v)
		{
			return vec4<T>(math::cos(v.x), math::cos(v.y), math::cos(v.z));
		}
		template<typename T>
		inline vec4<T> sin(const vec4<T>& v)
		{
			return vec4<T>(math::sin(v.x), math::sin(v.y), math::sin(v.z));
		}
		template<typename T>
		inline vec4<T> tan(const vec4<T>& v)
		{
			return vec4<T>(math::tan(v.x), math::tan(v.y), math::tan(v.z));
		}
		template<typename T>
		inline vec4<T> acos(const vec4<T>& v)
		{
			return vec4<T>(math::acos(v.x), math::acos(v.y), math::acos(v.z));
		}
		template<typename T>
		inline vec4<T> asin(const vec4<T>& v)
		{
			return vec4<T>(math::asin(v.x), math::asin(v.y), math::asin(v.z));
		}
		template<typename T>
		inline vec4<T> atan(const vec4<T>& v)
		{
			return vec4<T>(math::atan(v.x), math::atan(v.y), math::atan(v.z));
		}
		template<typename T>
		inline vec4<T> atan2(const vec4<T>& y, const vec4<T>& x)
		{
			return vec4<T>(math::atan2(y.x, x.x), math::atan2(y.y, x.y), math::atan2(y.z, x.z), math::atan2(y.w, x.w));
		}
		template<typename T>
		inline void sincos(const vec4<T>& angle, vec4<T>& s, vec4<T>& c)
		{
			s = math::sin(angle);
			c = math::cos(angle);
		}

	}
}