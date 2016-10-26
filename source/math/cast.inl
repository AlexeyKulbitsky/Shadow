namespace sh
{
	namespace math
	{

		template<typename T>
		inline vec2<T>::vec2(const vec3<T>& v) : x(v.x), y(v.y) {}

		template<typename T>
		inline vec2<T>::vec2(const vec4<T>& v) : x(v.x), y(v.y) {}

		template<typename T>
		inline vec2<T>& vec2<T>::operator=(const vec3<T>& v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		template<typename T>
		inline vec2<T>& vec2<T>::operator=(const vec4<T>& v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline vec3<T>::vec3(const vec2<T>& v) : x(v.x), y(v.y), z(0) {}

		template<typename T>
		inline vec3<T>::vec3(const vec2<T>& v, T z) : x(v.x), y(v.y), z(z) {}

		template<typename T>
		inline vec3<T>::vec3(const vec4<T>& v) : x(v.x), y(v.y), z(v.z) {}

		template<typename T>
		inline vec3<T>& vec3<T>::operator=(const vec2<T>& v)
		{
			x = v.x;
			y = v.y;
			z = 0;
			return *this;
		}

		template<typename T>
		inline vec3<T>& vec3<T>::operator=(const vec4<T>& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline vec4<T>::vec4(const vec2<T>& v) : x(v.x), y(v.y), z(0), w(0) {}

		template<typename T>
		inline vec4<T>::vec4(const vec2<T>& v, T z, T w) : x(v.x), y(v.y), z(z), w(w) {}

		template<typename T>
		inline vec4<T>::vec4(const vec3<T>& v) : x(v.x), y(v.y), z(v.z), w(0) {}

		template<typename T>
		inline vec4<T>::vec4(const vec3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}

		template<typename T>
		inline vec4<T>& vec4<T>::operator=(const vec2<T>& v)
		{
			x = v.x;
			y = v.y;
			z = 0;
			w = 0;
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline mat4<T>::mat4(const mat3<T>& v)
		{
			*this = v;
		}

		template<typename T>
		inline mat4<T>& mat4<T>::operator=(const mat3<T>& v)
		{
			mat3<T> t(v);
			setRotationPart(t);
			m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0;
			m[15] = 1;
			return *this;
		}

		template<typename T>
		inline mat4<T>::mat4(const mat2<T>& v)
		{
			*this = v;
		}

		template<typename T>
		inline mat4<T>& mat4<T>::operator=(const mat2<T>& v)
		{
			mat3<T> t(v);
			setRotationPart(t);
			m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0;
			m[15] = 1;
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline mat3<T>::mat3(const mat4<T>& v)
		{
			*this = v;
		}

		template<typename T>
		inline mat3<T>& mat3<T>::operator=(const mat4<T>& v)
		{
			m[0] = v.m[0];
			m[1] = v.m[1];
			m[2] = v.m[2];
			m[3] = v.m[4];
			m[4] = v.m[5];
			m[5] = v.m[6];
			m[6] = v.m[8];
			m[7] = v.m[9];
			m[8] = v.m[10];
			return *this;
		}

		template<typename T>
		inline mat3<T>::mat3(const mat2<T>& v)
		{
			*this = v;
		}

		template<typename T>
		inline mat3<T>& mat3<T>::operator=(const mat2<T>& v)
		{
			m[0] = v.m[0];
			m[1] = v.m[1];
			m[2] = 0;
			m[3] = v.m[2];
			m[4] = v.m[3];
			m[5] = 0;
			m[6] = 0;
			m[7] = 0;
			m[8] = 1;
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline mat2<T>::mat2(const mat3<T>& v)
		{
			*this = v;
		}

		template<typename T>
		inline mat2<T>& mat2<T>::operator=(const mat3<T>& v)
		{
			m[0] = v.m[0];
			m[1] = v.m[1];
			m[2] = v.m[3];
			m[3] = v.m[4];
			return *this;
		}

		template<typename T>
		inline mat2<T>::mat2(const mat4<T>& v)
		{
			*this = v;
		}

		template<typename T>
		inline mat2<T>& mat2<T>::operator=(const mat4<T>& v)
		{
			*this = mat3<T>(v);
			return *this;
		}

	}
}