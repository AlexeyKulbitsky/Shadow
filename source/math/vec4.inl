namespace sh
{
	namespace math
	{



		template<typename T>
		inline vec4<T>::vec4() : x(T(0)), y(T(0)), z(T(0)), w(T(0)) {}

		template<typename T>
		inline vec4<T>::vec4(ZUninitialized) {}

		template<typename T>
		inline vec4<T>::vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

		template<typename T>
		inline vec4<T>::vec4(T v) : x(v), y(v), z(v), w(v) {}

		template<typename T>
		inline vec4<T>::vec4(const vec4<T>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

		template<typename T>
		template<typename U>
		inline vec4<T>::vec4(const vec4<U>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) {}

		template<typename T>
		inline vec4<T>& vec4<T>::set(T _x, T _y, T _z, T _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
			return *this;
		}

		template<typename T>
		inline T vec4<T>::getLength() const
		{
			return math::sqrt(getLengthSq());
		}
		template<typename T>
		inline T vec4<T>::getLengthSq() const
		{
			return x*x + y*y + z*z + w*w;
		}

		template <typename T>
		inline void vec4<T>::setLength(T len)
		{
			T oldLen = getLength();
			*this *= (len / oldLen);
		}

		template <typename T>
		inline vec4<T>& vec4<T>::normalize()
		{
			T len = getLength();
			if (isZero(len))
			{
				return *this;
			}
			T invLen = ((T)1.0) / len;
			*this *= invLen;
			return *this;
		}

		template<typename T>
		inline bool vec4<T>::operator==(const vec4<T>& v) const
		{
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}

		template<typename T>
		inline bool vec4<T>::operator!=(const vec4<T>& v) const
		{
			return !operator==(v);
		}

		template<typename T>
		inline vec4<T> vec4<T>::operator-() const
		{
			return vec4<T>(-x, -y, -z, -w);
		}

		template<typename T>
		inline T& vec4<T>::operator[](uint32_t i)
		{
			assert(i < sizeof(*this) / sizeof(T));

			//if you hit this assert, your compiler introduces padding. Check for #pragma packs without pop
			assert(&((T*)this)[0] == &x && &((T*)this)[1] == &y);

			return ((T*)this)[i];
		}

		template<typename T>
		inline const T& vec4<T>::operator[](uint32_t i) const
		{
			assert(i < sizeof(*this) / sizeof(T));

			//if you hit this assert, your compiler introduces padding. Check for #pragma packs without pop
			assert(&((T*)this)[0] == &x && &((T*)this)[1] == &y);

			return ((T*)this)[i];
		}

		template<typename T>
		inline vec4<T> vec4<T>::operator+(T s) const
		{
			return vec4<T>(x + s, y + s, z + s, w + s);
		}
		template<typename T>
		inline vec4<T> vec4<T>::operator+(const vec4<T>& v) const
		{
			return vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
		}

		template<typename T>
		inline vec4<T> vec4<T>::operator-(T s) const
		{
			return vec4<T>(x - s, y - s, z - s, w - s);
		}
		template<typename T>
		inline vec4<T> vec4<T>::operator-(const vec4<T>& v) const
		{
			return vec4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
		}

		template<typename T>
		inline vec4<T> vec4<T>::operator*(T s) const
		{
			return vec4<T>(x * s, y * s, z * s, w * s);
		}

		template<typename T>
		inline vec4<T> vec4<T>::operator/(T s) const
		{
			assert(s != T(0));
			return vec4<T>(x / s, y / s, z / s, w / s);
		}
		template<>
		inline vec4<float> vec4<float>::operator/(float s) const
		{
			assert(s != float(0));
			float ts = float(1) / s;
			return vec4<float>(x * ts, y * ts, z * ts, w * ts);
		}
		template<>
		inline vec4<double> vec4<double>::operator/(double s) const
		{
			assert(s != double(0));
			double ts = double(1) / s;
			return vec4<double>(x * ts, y * ts, z * ts, w * ts);
		}


		template<typename T>
		inline vec4<T>& vec4<T>::operator=(const vec4<T>& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
			return *this;
		}

		template<typename T>
		inline vec4<T>& vec4<T>::operator=(const vec3<T>& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = 0;
			return *this;
		}

		template<typename T>
		inline vec4<T>& vec4<T>::operator+=(T s)
		{
			x += s;
			y += s;
			z += s;
			w += s;
			return *this;
		}
		template<typename T>
		inline vec4<T>& vec4<T>::operator+=(const vec4<T>& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		template<typename T>
		inline vec4<T>& vec4<T>::operator-=(T s)
		{
			x -= s;
			y -= s;
			z -= s;
			w -= s;
			return *this;
		}
		template<typename T>
		inline vec4<T>& vec4<T>::operator-=(const vec4<T>& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		template<typename T>
		inline vec4<T>& vec4<T>::operator*=(T s)
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}
		template<typename T>
		inline vec4<T>& vec4<T>::operator*=(const vec4<T>& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;
			return *this;
		}

		template<typename T>
		inline vec4<T>& vec4<T>::operator/=(T s)
		{
			assert(s != T(0));
			x /= s;
			y /= s;
			z /= s;
			w /= s;
			return *this;
		}
		template<>
		inline vec4<float>& vec4<float>::operator/=(float s)
		{
			assert(s != float(0));
			float ts = float(1) / s;
			x *= ts;
			y *= ts;
			z *= ts;
			w *= ts;
			return *this;
		}
		template<>
		inline vec4<double>& vec4<double>::operator/=(double s)
		{
			assert(s != double(0));
			double ts = double(1) / s;
			x *= ts;
			y *= ts;
			z *= ts;
			w *= ts;
			return *this;
		}

		template<typename T>
		inline vec4<T>& vec4<T>::operator/=(const vec4<T>& v)
		{
			assert(v.x != T(0) && v.y != T(0) && v.z != T(0) && v.w != T(0));
			x /= v.x;
			y /= v.y;
			z /= v.z;
			w /= v.w;
			return *this;
		}

		template <typename T>
		inline vec4<T> operator*(T s, const vec4<T>& v)
		{
			return v*s;
		}
		template <typename T>
		inline vec4<T> operator/(T s, const vec4<T>& v)
		{
			return vec4<T>(s) / v;
		}

		template<typename T>
		inline vec4<T> operator*(const vec4<T>& v0, const vec4<T>& v1)
		{
			return vec4<T>(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w);
		}

		template<typename T>
		inline vec4<T> operator/(const vec4<T>& u, const vec4<T>& v)
		{
			assert(v.x != T(0) && v.y != T(0) && v.z != T(0) && v.w != T(0));
			return vec4<T>(u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w);
		}


	}
}