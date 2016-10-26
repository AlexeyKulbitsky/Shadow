namespace sh
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////
		//constructors
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat2<T>::mat2()
		{
			setIdentity();
		}

		template <typename T>
		inline mat2<T>::mat2(ZUninitialized)
		{
		}

		template <typename T>
		inline mat2<T>::mat2(T value)
		{
			m[0] = m[1] = value;
			m[2] = m[3] = value;
		}

		template <typename T>
		inline mat2<T>::mat2(const mat2<T>& other)
		{
			*this = other;
		}

		template<typename T>
		template<typename U>
		inline mat2<T>::mat2(const mat2<U>& other)
		{
			m[0] = (T)other.m[0];
			m[1] = (T)other.m[1];
			m[2] = (T)other.m[2];
			m[3] = (T)other.m[3];
		}

		template<typename T>
		inline mat2<T>::mat2(const T values[4])
		{
			set(values);
		}

		///////////////////////////////////////////////////////////////////////////////
		//methods
		///////////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline mat2<T>& mat2<T>::set(const T values[4])
		{
			assert(values);
			memcpy(m, values, sizeof(T) * 4);
			return *this;
		}

		template <typename T>
		inline mat2<T>& mat2<T>::setIdentity()
		{
			m[0] = m[3] = (T)1;
			m[1] = m[2] = (T)0;
			return *this;
		}

		template <typename T>
		inline bool mat2<T>::invert()
		{
			// http://www.geometrictools.com/LibFoundation/Mathematics/Wm4Matrix3.inl
			// Invert a 3x3 using cofactors.  This is faster than using a generic
			// Gaussian elimination because of the loop overhead of such a method.

			assert(0 && "not implemented");
			return true;
		}

		template <typename T>
		inline mat2<T>& mat2<T>::transpose()
		{
			std::swap(m[1], m[2]);
			return *this;
		}

		template <typename T>
		inline vec2<T> mat2<T>::getRow(uint32_t row) const
		{
			assert(row < 2);
			uint32_t idx = row * 2;
			return vec2<T>(m[idx], m[idx + 1]);
		}
		template <typename T>
		inline mat2<T>& mat2<T>::setRow(uint32_t row, const vec2<T>& v)
		{
			assert(row < 2);
			uint32_t idx = row * 2;
			m[idx] = v.x;
			m[idx + 1] = v.y;
			return *this;
		}

		template <typename T>
		inline vec2<T> mat2<T>::getColumn(uint32_t column) const
		{
			assert(column < 2);
			uint32_t idx = column;// * 2;
			return vec2<T>(m[idx], m[idx + 2]);
		}
		template <typename T>
		inline mat2<T>& mat2<T>::setColumn(uint32_t column, const vec2<T>& v)
		{
			assert(column < 2);
			uint32_t idx = column;//  * 2;
			m[idx] = v.x;
			m[idx + 2] = v.y;
			return *this;
		}

		template <typename T>
		inline vec2<T> mat2<T>::getAxisX() const
		{
			return vec2<T>(m[0], m[1]);
		}

		template <typename T>
		inline vec2<T> mat2<T>::getAxisY() const
		{
			return vec2<T>(m[2], m[3]);
		}

		template <typename T>
		inline vec2<T> mat2<T>::getScale() const
		{
			return vec2<T>(getAxisX().length(), getAxisY().length());
		}

		template <typename T>
		inline mat2<T>& mat2<T>::setAxisX(const vec2<T>& axis)
		{
			m[0] = axis.x;
			m[1] = axis.y;
			return *this;
		}

		template <typename T>
		inline mat2<T>& mat2<T>::setAxisY(const vec2<T>& axis)
		{
			m[2] = axis.x;
			m[3] = axis.y;
			return *this;
		}

		template <typename T>
		inline mat2<T>& mat2<T>::setScale(const vec2<T>& s)
		{
			m[0] = s.x;
			m[3] = s.y;
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline bool mat2<T>::operator==(const mat2<T>& m) const
		{
			return memcmp(this->m, m.m, sizeof(T) * 4) == 0;
		}
		template <typename T>
		inline bool mat2<T>::operator!=(const mat2<T>& m) const
		{
			return !operator==(m);
		}

		///////////////////////////////////////////////////////////////////////////////
		// indexing operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat2<T>::operator T*()
		{
			return m;
		}
		template <typename T>
		inline mat2<T>::operator const T*() const
		{
			return m;
		}

		template <typename T>
		inline T& mat2<T>::operator()(uint32_t row, uint32_t col)
		{
			assert(row * 2 + col < 4);
			return m[row * 2 + col];
		}

		template <typename T>
		inline const T& mat2<T>::operator()(uint32_t row, uint32_t col) const
		{
			assert(row * 2 + col < 4);
			return m[row * 2 + col];
		}

		///////////////////////////////////////////////////////////////////////////////
		// scalar operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat2<T> mat2<T>::operator*(T scalar) const
		{
			mat2<T> temp(uninitialized);
			temp.m[0] = m[0] * scalar;
			temp.m[1] = m[1] * scalar;
			temp.m[2] = m[2] * scalar;
			temp.m[3] = m[3] * scalar;
			return temp;
		}

		template <typename T>
		inline mat2<T>& mat2<T>::operator*=(T scalar)
		{
			m[0] *= scalar;
			m[1] *= scalar;
			m[2] *= scalar;
			m[3] *= scalar;
			return *this;
		}

		///////////////////////////////////////////////////////////////////////////////
		// arithmetic operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat2<T> mat2<T>::operator*(const mat2<T>& other) const
		{
			mat2<T> ret(uninitialized);
			const T* m1 = m;
			const T* m2 = other.m;
			T* m3 = ret.m;
			// using this code only on release and when on the iphone
			m3[0] = m1[0] * m2[0] + m1[2] * m2[1];
			m3[1] = m1[1] * m2[0] + m1[3] * m2[1];

			m3[2] = m1[0] * m2[2] + m1[2] * m2[1];
			m3[3] = m1[1] * m2[2] + m1[3] * m2[1];

			return ret;
		}

		template <typename T>
		inline mat2<T> mat2<T>::operator+(const mat2<T>& other) const
		{
			mat2<T> temp(uninitialized);
			temp[0] = m[0] + other[0];
			temp[1] = m[1] + other[1];
			temp[2] = m[2] + other[2];
			temp[3] = m[3] + other[3];
			return temp;
		}

		template <typename T>
		inline mat2<T> mat2<T>::operator-(const mat2<T>& other) const
		{
			mat2<T> temp(uninitialized);
			temp.m[0] = m[0] - other.m[0];
			temp.m[1] = m[1] - other.m[1];
			temp.m[2] = m[2] - other.m[2];
			temp.m[3] = m[3] - other.m[3];
			return temp;
		}

		// template <typename T>
		// inline vec2<T> mat2<T>::operator*(const vec2<T>& p) const
		// {
		// 	T px = p.x;
		// 	T py = p.y;
		// 	T x = px*m[0] + py*m[2];
		// 	T y = px*m[1] + py*m[3];
		// 	return vec2<T>(x, y);
		// }

		template <typename T>
		mat2<T>& mat2<T>::operator+=(const mat2<T>& other)
		{
			m[0] += other.m[0];
			m[1] += other.m[1];
			m[2] += other.m[2];
			m[3] += other.m[3];
			return *this;
		}

		template <typename T>
		mat2<T>& mat2<T>::operator-=(const mat2<T>& other)
		{
			m[0] -= other.m[0];
			m[1] -= other.m[1];
			m[2] -= other.m[2];
			m[3] -= other.m[3];
			return *this;
		}


	}
}