
namespace sh
{
	namespace math
	{

		template<typename T>
		inline vec3<T>::vec3() : x(T(0)), y(T(0)), z(T(0)) {}

		template<typename T>
		inline vec3<T>::vec3(ZUninitialized) {}

		template<typename T>
		inline vec3<T>::vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

		template<typename T>
		inline vec3<T>::vec3(T v) : x(v), y(v), z(v) {}

		template<typename T>
		inline vec3<T>::vec3(const vec3<T>& v) : x(v.x), y(v.y), z(v.z) {}

		template<typename T>
		template<typename U>
		inline vec3<T>::vec3(const vec3<U>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)) {}


		template<typename T>
		inline vec3<T>& vec3<T>::set(T _x, T _y, T _z)
		{
			x = _x;
			y = _y;
			z = _z;
			return *this;
		}

		template<typename T>
		inline T vec3<T>::getLength() const
		{
			return math::sqrt(getLengthSq());
		}

		template<typename T>
		inline T vec3<T>::getLengthSq() const
		{
			return x*x + y*y + z*z;
		}

		template <typename T>
		inline void vec3<T>::setLength(T len)
		{
			T oldLen = getLength();
			assert(oldLen != 0);
			*this *= (len / oldLen);
		}

		template <typename T>
		inline vec3<T>& vec3<T>::normalize()
		{
			T len = getLength();
			if (math::isZero(len))
			{
				return *this;
			}
			T invLen = ((T)1) / len;
			*this *= invLen;
			return *this;
		}

		template<typename T>
		inline bool vec3<T>::operator==(const vec3<T>& v) const
		{
			return x == v.x && y == v.y && z == v.z;
		}

		template<typename T>
		inline bool vec3<T>::operator!=(const vec3<T>& v) const
		{
			return !operator==(v);
		}

		template<typename T>
		inline T& vec3<T>::operator[](uint32_t i)
		{
			assert(i < sizeof(*this) / sizeof(T));

			//if you hit this assert, your compiler introduces padding. Check for #pragma packs without pop
			assert(&((T*)this)[0] == &x && &((T*)this)[1] == &y);

			return ((T*)this)[i];
		}

		template<typename T>
		inline const T& vec3<T>::operator[](uint32_t i) const
		{
			assert(i < sizeof(*this) / sizeof(T));

			//if you hit this assert, your compiler introduces padding. Check for #pragma packs without pop
			assert(&((T*)this)[0] == &x && &((T*)this)[1] == &y);

			return ((T*)this)[i];
		}

		template<typename T>
		inline vec3<T> vec3<T>::operator-() const
		{
			return vec3<T>(-x, -y, -z);
		}

		template<typename T>
		inline vec3<T> vec3<T>::operator+(T s) const
		{
			return vec3<T>(x + s, y + s, z + s);
		}
		template<typename T>
		inline vec3<T> vec3<T>::operator+(const vec3<T>& v2) const
		{
			return vec3<T>(x + v2.x, y + v2.y, z + v2.z);
		}

		template<typename T>
		inline vec3<T> vec3<T>::operator-(T s) const
		{
			return vec3<T>(x - s, y - s, z - s);
		}
		template<typename T>
		inline vec3<T> vec3<T>::operator-(const vec3<T>& v2) const
		{
			return vec3<T>(x - v2.x, y - v2.y, z - v2.z);
		}

		template<typename T>
		inline vec3<T> vec3<T>::operator*(T s) const
		{
			return vec3<T>(x * s, y * s, z * s);
		}

		template<typename T>
		inline vec3<T> vec3<T>::operator/(T s) const
		{
			assert(s != 0);
			return vec3<T>(x / s, y / s, z / s);
		}
		template<>
		inline vec3<float> vec3<float>::operator/(float s) const
		{
			assert(s != 0);
			float ts = 1.f / s;
			return vec3<float>(x * ts, y * ts, z * ts);
		}
		template<>
		inline vec3<double> vec3<double>::operator/(double s) const
		{
			assert(s != 0);
			double ts = 1.0 / s;
			return vec3<double>(x * ts, y * ts, z * ts);
		}

		template<typename T>
		inline vec3<T>& vec3<T>::operator=(const vec3<T>& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		template<typename T>
		inline vec3<T>& vec3<T>::operator+=(T s)
		{
			x += s;
			y += s;
			z += s;
			return *this;
		}
		template<typename T>
		inline vec3<T>& vec3<T>::operator+=(const vec3<T>& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		template<typename T>
		inline vec3<T>& vec3<T>::operator-=(T s)
		{
			x -= s;
			y -= s;
			z -= s;
			return *this;
		}
		template<typename T>
		inline vec3<T>& vec3<T>::operator-=(const vec3<T>& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		template<typename T>
		inline vec3<T>& vec3<T>::operator*=(T s)
		{
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}
		template<typename T>
		inline vec3<T>& vec3<T>::operator*=(const vec3<T>& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		template<typename T>
		inline vec3<T>& vec3<T>::operator/=(T s)
		{
			assert(s != 0);
			x /= s;
			y /= s;
			z /= s;
			return *this;
		}

		template <>
		inline vec3<float>& vec3<float>::operator/=(float s)
		{
			assert(s != 0);
			float ts = 1.f / s;
			x *= ts;
			y *= ts;
			z *= ts;
			return *this;
		}
		template <>
		inline vec3<double>& vec3<double>::operator/=(double s)
		{
			assert(s != 0);
			double ts = 1.0 / s;
			x *= ts;
			y *= ts;
			z *= ts;
			return *this;
		}

		template<typename T>
		inline vec3<T>& vec3<T>::operator/=(const vec3<T>& v)
		{
			assert(v.x != T(0) && v.y != T(0) && v.z != T(0));
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}

		template <typename T>
		inline vec3<T> operator*(T s, const vec3<T>& v)
		{
			return v*s;
		}

		template<typename T>
		inline vec3<T> operator*(const vec3<T>& v0, const vec3<T>& v1)
		{
			return vec3<T>(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z);
		}

		template <typename T>
		inline vec3<T> operator/(T s, const vec3<T>& v)
		{
			return vec3<T>(s) / v;
		}

		template <typename T>
		inline vec3<T> operator/(const vec3<T>& u, const vec3<T>& v)
		{
			assert(v.x != T(0) && v.y != T(0) && v.z != T(0));
			return vec3<T>(u.x / v.x, u.y / v.y, u.z / v.z);
		}

	}
}