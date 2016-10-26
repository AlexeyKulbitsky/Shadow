namespace sh
{
	namespace math
	{
		template<typename T>
		inline vec2<T>::vec2() : x(T(0)), y(T(0)) {}

		template<typename T>
		inline vec2<T>::vec2(ZUninitialized) {}

		template<typename T>
		inline vec2<T>::vec2(T _x, T _y) : x(_x), y(_y) {}

		template<typename T>
		inline vec2<T>::vec2(T v) : x(v), y(v) {}

		template<typename T>
		inline vec2<T>::vec2(const vec2<T>& v) : x(v.x), y(v.y) {}

		template<typename T>
		template<typename U>
		inline vec2<T>::vec2(const vec2<U>& v) : x(T(v.x)), y(T(v.y)) {}

		template<typename T>
		inline vec2<T>& vec2<T>::set(T _x, T _y)
		{
			x = _x;
			y = _y;
			return *this;
		}

		template<typename T>
		inline T vec2<T>::getLength() const
		{
			return math::sqrt(getLengthSq());
		}
		template<typename T>
		inline T vec2<T>::getLengthSq() const
		{
			return x*x + y*y;
		}

		template <typename T>
		inline void vec2<T>::setLength(T len)
		{
			T oldLen = getLength();
			assert(oldLen != 0);
			*this *= (len / oldLen);
		}

		template <typename T>
		inline vec2<T>& vec2<T>::normalize()
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
		inline T& vec2<T>::operator[](uint32_t i)
		{
			assert(i < sizeof(*this) / sizeof(T));

			//if you hit this assert, your compiler introduces padding. Check for #pragma packs without pop
			assert(&((T*)this)[0] == &x && &((T*)this)[1] == &y);

			return ((T*)this)[i];
		}

		template<typename T>
		inline const T& vec2<T>::operator[](uint32_t i) const
		{
			assert(i < sizeof(*this) / sizeof(T));

			//if you hit this assert, your compiler introduces padding. Check for #pragma packs without pop
			assert(&((T*)this)[0] == &x && &((T*)this)[1] == &y);

			return ((T*)this)[i];
		}

		template<typename T>
		inline bool vec2<T>::operator==(const vec2<T>& v) const
		{
			return x == v.x && y == v.y;
		}

		template<typename T>
		inline bool vec2<T>::operator!=(const vec2<T>& v) const
		{
			return !operator==(v);
		}

		template<typename T>
		inline vec2<T> vec2<T>::operator-() const
		{
			return vec2<T>(-x, -y);
		}

		template<typename T>
		inline vec2<T> vec2<T>::operator+(T s) const
		{
			return vec2<T>(x + s, y + s);
		}
		template<typename T>
		inline vec2<T> vec2<T>::operator+(const vec2<T>& v) const
		{
			return vec2<T>(x + v.x, y + v.y);
		}

		template<typename T>
		inline vec2<T> vec2<T>::operator-(T s) const
		{
			return vec2<T>(x - s, y - s);
		}
		template<typename T>
		inline vec2<T> vec2<T>::operator-(const vec2<T>& v2) const
		{
			return vec2<T>(x - v2.x, y - v2.y);
		}

		template<typename T>
		inline vec2<T> vec2<T>::operator*(T s) const
		{
			return vec2<T>(x * s, y * s);
		}

		template<typename T>
		inline vec2<T> vec2<T>::operator/(T s) const
		{
			assert(s != T(0));
			return vec2<T>(x / s, y / s);
		}
		template<>
		inline vec2<float> vec2<float>::operator/(float s) const
		{
			assert(s != 0);
			float is = float(1) / s;
			return vec2<float>(x * is, y * is);
		}
		template<>
		inline vec2<double> vec2<double>::operator/(double s) const
		{
			assert(s != 0);
			double is = double(1) / s;
			return vec2<double>(x * is, y * is);
		}

		template<typename T>
		inline vec2<T>& vec2<T>::operator=(const vec2<T>& v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		template<typename T>
		inline vec2<T>& vec2<T>::operator+=(T s)
		{
			x += s;
			y += s;
			return *this;
		}
		template<typename T>
		inline vec2<T>& vec2<T>::operator+=(const vec2<T>& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		template<typename T>
		inline vec2<T>& vec2<T>::operator-=(T s)
		{
			x -= s;
			y -= s;
			return *this;
		}
		template<typename T>
		inline vec2<T>& vec2<T>::operator-=(const vec2<T>& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		template<typename T>
		inline vec2<T>& vec2<T>::operator*=(T s)
		{
			x *= s;
			y *= s;
			return *this;
		}
		template<typename T>
		inline vec2<T>& vec2<T>::operator*=(const vec2<T>& v)
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}

		template<typename T>
		inline vec2<T>& vec2<T>::operator/=(T s)
		{
			assert(s != T(0));
			x /= s;
			y /= s;
			return *this;
		}
		template<>
		inline vec2<float>& vec2<float>::operator/=(float s)
		{
			assert(s != float(0));
			float ts = float(1) / s;
			x *= ts;
			y *= ts;
			return *this;
		}
		template<>
		inline vec2<double>& vec2<double>::operator/=(double s)
		{
			assert(s != double(0));
			double ts = double(1) / s;
			x *= ts;
			y *= ts;
			return *this;
		}

		template<typename T>
		inline vec2<T>& vec2<T>::operator/=(const vec2<T>& v)
		{
			assert(v.x != T(0) && v.y != T(0));
			x /= v.x;
			y /= v.y;
			return *this;
		}

		template <typename T>
		inline vec2<T> operator*(T s, const vec2<T>& v)
		{
			return v*s;
		}
		template <typename T>
		inline vec2<T> operator/(T s, const vec2<T>& v)
		{
			return vec2<T>(s) / v;
		}

		template<typename T>
		inline vec2<T> operator*(const vec2<T>& v0, const vec2<T>& v1)
		{
			return vec2<T>(v0.x * v1.x, v0.y * v1.y);
		}

		template<typename T>
		inline vec2<T> operator/(const vec2<T>& u, const vec2<T>& v)
		{
			assert(v.x != T(0) && v.y != T(0));
			return vec2<T>(u.x / v.x, u.y / v.y);
		}

	}
}

