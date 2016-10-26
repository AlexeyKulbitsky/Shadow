namespace sh
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////
		//constructors
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat3<T>::mat3()
		{
			setIdentity();
		}

		template <typename T>
		inline mat3<T>::mat3(ZUninitialized)
		{
		}

		template <typename T>
		inline mat3<T>::mat3(T value)
		{
			m[0] = m[1] = m[2] = value;
			m[3] = m[4] = m[5] = value;
			m[6] = m[7] = m[8] = value;
		}

		template <typename T>
		inline mat3<T>::mat3(const mat3<T>& other)
		{
			*this = other;
		}

		template<typename T>
		template<typename U>
		inline mat3<T>::mat3(const mat3<U>& other)
		{
			m[0] = (T)other.m[0];
			m[1] = (T)other.m[1];
			m[2] = (T)other.m[2];
			m[3] = (T)other.m[3];
			m[4] = (T)other.m[4];
			m[5] = (T)other.m[5];
			m[6] = (T)other.m[6];
			m[7] = (T)other.m[7];
			m[8] = (T)other.m[8];
		}

		template<typename T>
		inline mat3<T>::mat3(const T values[9])
		{
			set(values);
		}

		///////////////////////////////////////////////////////////////////////////////
		//methods
		///////////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline mat3<T>& mat3<T>::set(const T values[9])
		{
			assert(values);
			memcpy(m, values, sizeof(T) * 9);
			return *this;
		}

		template <typename T>
		inline mat3<T>& mat3<T>::setIdentity()
		{
			setRotationPartIdentity();
			m[8] = (T)1;
			m[6] = m[7] = (T)0;
			return *this;
		}

		template <typename T>
		inline mat3<T>& mat3<T>::setRotationIdentity()
		{
			MDEPRECATED("Will be removed on 1 March 2012. Please replace with setRotationPartIdentity()");
			return setRotationPartIdentity();
		}
		template <typename T>
		inline mat3<T>& mat3<T>::setRotationPartIdentity()
		{
			m[0] = m[4] = (T)1;
			m[1] = m[2] = m[3] = m[5] = (T)0;
			return *this;
		}

		template <typename T>
		inline bool mat3<T>::invert()
		{
			// http://www.geometrictools.com/LibFoundation/Mathematics/Wm4Matrix3.inl
			// Invert a 3x3 using cofactors.  This is faster than using a generic
			// Gaussian elimination because of the loop overhead of such a method.

			mat3<T> inv;
			inv.m[0] = m[4] * m[8] - m[5] * m[7];
			inv.m[1] = m[2] * m[7] - m[1] * m[8];
			inv.m[2] = m[1] * m[5] - m[2] * m[4];
			inv.m[3] = m[5] * m[6] - m[3] * m[8];
			inv.m[4] = m[0] * m[8] - m[2] * m[6];
			inv.m[5] = m[2] * m[3] - m[0] * m[5];
			inv.m[6] = m[3] * m[7] - m[4] * m[6];
			inv.m[7] = m[1] * m[6] - m[0] * m[7];
			inv.m[8] = m[0] * m[4] - m[1] * m[3];

			T fDet =
				m[0] * inv.m[0] +
				m[1] * inv.m[3] +
				m[2] * inv.m[6];

			if (isZero(fDet))
			{
				return false;
			}

			T fInvDet = T(1) / fDet;
			inv.m[0] *= fInvDet;
			inv.m[1] *= fInvDet;
			inv.m[2] *= fInvDet;
			inv.m[3] *= fInvDet;
			inv.m[4] *= fInvDet;
			inv.m[5] *= fInvDet;
			inv.m[6] *= fInvDet;
			inv.m[7] *= fInvDet;
			inv.m[8] *= fInvDet;

			*this = inv;
			return true;
		}

		template <typename T>
		inline mat3<T>& mat3<T>::transpose()
		{
			std::swap(m[1], m[3]);
			std::swap(m[2], m[6]);
			std::swap(m[5], m[7]);
			return *this;
		}

		template <typename T>
		inline vec3<T> mat3<T>::getRow(uint32_t row) const
		{
			assert(row < 3);
			uint32_t idx = row * 3;
			return vec3<T>(m[idx + 0], m[idx + 1], m[idx + 2]);
		}
		template <typename T>
		inline void mat3<T>::setRow(uint32_t row, const vec3<T>& v)
		{
			assert(row < 3);
			uint32_t idx = row * 3;
			m[idx + 0] = v.x;
			m[idx + 1] = v.y;
			m[idx + 2] = v.z;
		}

		template <typename T>
		inline vec3<T> mat3<T>::getColumn(uint32_t column) const
		{
			assert(column < 3);
			uint32_t idx = column;
			return vec3<T>(m[idx + 0], m[idx + 3], m[idx + 6]);
		}
		template <typename T>
		inline mat3<T>& mat3<T>::setColumn(uint32_t column, const vec3<T>& v)
		{
			assert(column < 3);
			uint32_t idx = column;
			m[idx + 0] = v.x;
			m[idx + 3] = v.y;
			m[idx + 6] = v.z;
			return *this;
		}

		template <typename T>
		inline vec2<T> mat3<T>::getAxisX() const
		{
			return vec2<T>(m[0], m[1]);
		}

		template <typename T>
		inline vec2<T> mat3<T>::getAxisY() const
		{
			return vec2<T>(m[3], m[4]);
		}

		template <typename T>
		inline const vec2<T>& mat3<T>::getTranslation() const
		{
			return *(vec2<T>*)(&m[6]);
		}

		template<typename T>
		inline T mat3<T>::getRotation() const
		{
			MDEPRECATED("Will be removed on 1 March 2012. Please replace with getRotationPart()");
			assert(0);
			return 0;
		}
		template<typename T>
		inline T mat3<T>::getRotationPart() const
		{
			MTODO("to implement - contact the engine team()");
			assert(0);
			return 0;
		}
		template<typename T>
		inline mat3<T>&	mat3<T>::setRotation(T rot)
		{
			MDEPRECATED("Will be removed on 1 March 2012. Please replace with setRotationPart()");
			return setRotationPart(rot);
		}
		template<typename T>
		inline mat3<T>&	mat3<T>::setRotationPart(T rot)
		{
			T c = math::cos(rot);
			T s = math::sin(rot);
			m[0] = c;
			m[1] = s;
			m[3] = -s;
			m[4] = c;
			return *this;
		}

		template <typename T>
		inline vec2<T> mat3<T>::getScale() const
		{
			return vec2<T>(getAxisX().getLength(), getAxisY().getLength());
		}

		template <typename T>
		inline mat3<T>& mat3<T>::setAxisX(const vec2<T>& axis)
		{
			m[0] = axis.x;
			m[1] = axis.y;
			return *this;
		}

		template <typename T>
		inline mat3<T>& mat3<T>::setAxisY(const vec2<T>& axis)
		{
			m[3] = axis.x;
			m[4] = axis.y;
			return *this;
		}

		template <typename T>
		inline mat3<T>& mat3<T>::setTranslation(const vec2<T>& p)
		{
			m[6] = p.x;
			m[7] = p.y;
			return *this;
		}

		template <typename T>
		inline mat3<T>& mat3<T>::setScale(const vec2<T>& s)
		{
			m[0] = s.x;
			m[4] = s.y;
			return *this;
		}

		template <typename T>
		inline mat3<T>& mat3<T>::postScale(const vec2<T>& scale)
		{
			m[0] *= scale.x;
			m[1] *= scale.x;
			m[3] *= scale.y;
			m[4] *= scale.y;
			return *this;
		}

		template <typename T>
		inline mat3<T>& mat3<T>::postTranslate(const vec2<T>& trans)
		{
			m[6] += m[0] * trans.x + m[3] * trans.y;
			m[7] += m[1] * trans.x + m[4] * trans.y;
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline bool mat3<T>::operator==(const mat3<T>& m) const
		{
			return memcmp(this->m, m.m, sizeof(T) * 9) == 0;
		}
		template <typename T>
		inline bool mat3<T>::operator!=(const mat3<T>& m) const
		{
			return !operator==(m);
		}

		///////////////////////////////////////////////////////////////////////////////
		// indexing operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat3<T>::operator T*()
		{
			return m;
		}
		template <typename T>
		inline mat3<T>::operator const T*() const
		{
			return m;
		}

		template <typename T>
		inline T& mat3<T>::operator()(uint32_t row, uint32_t col)
		{
			assert(row * 3 + col < 9);
			return m[row * 3 + col];
		}

		template <typename T>
		inline const T& mat3<T>::operator()(uint32_t row, uint32_t col) const
		{
			assert(row * 3 + col < 9);
			return m[row * 3 + col];
		}

		///////////////////////////////////////////////////////////////////////////////
		// scalar operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat3<T> mat3<T>::operator*(T scalar) const
		{
			mat3<T> temp(uninitialized);
#ifdef XXXUSE_NEON_ASM
			asm volatile
				(
					"vldmia %0, { q0-q3 }	\n\t"
					"vdup.float q4, %1    	\n\t" // vdup splates a scalar into a vector

					"vmul.float q0, q0, q4	\n\t"
					"vmul.float q1, q1, q4	\n\t"
					"vmul.float q2, q2, q4	\n\t"
					"vmul.float q3, q3, q4	\n\t"

					"vstmia %2, { q0-q3 }	\n\t"
					: // no output
			: "r" (m), "r" (scalar), "r" (temp.m)     // input
				: "memory",
				"q0", "q1", "q2", "q3", "q4"
				);
#else
			temp.m[0] = m[0] * scalar;
			temp.m[1] = m[1] * scalar;
			temp.m[2] = m[2] * scalar;
			temp.m[3] = m[3] * scalar;
			temp.m[4] = m[4] * scalar;
			temp.m[5] = m[5] * scalar;
			temp.m[6] = m[6] * scalar;
			temp.m[7] = m[7] * scalar;
			temp.m[8] = m[8] * scalar;
#endif
			return temp;
		}

		template <typename T>
		inline mat3<T>& mat3<T>::operator*=(T scalar)
		{
#ifdef XXXUSE_NEON_ASM
			asm volatile
				(
					"vldmia %0, { q0-q3 }	\n\t"
					"vdup.float q4, %1    	\n\t" // vdup splates a scalar into a vector

					"vmul.float q0, q0, q4	\n\t"
					"vmul.float q1, q1, q4	\n\t"
					"vmul.float q2, q2, q4	\n\t"
					"vmul.float q3, q3, q4	\n\t"

					"vstmia %0, { q0-q3 }	\n\t"
					: // no output
			: "r" (m), "r" (scalar)     // input
				: "memory",
				"q0", "q1", "q2", "q3", "q4"
				);
#else
			m[0] *= scalar;
			m[1] *= scalar;
			m[2] *= scalar;
			m[3] *= scalar;
			m[4] *= scalar;
			m[5] *= scalar;
			m[6] *= scalar;
			m[7] *= scalar;
			m[8] *= scalar;
#endif
			return *this;
		}

		///////////////////////////////////////////////////////////////////////////////
		// arithmetic operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat3<T> mat3<T>::operator*(const mat3<T>& other) const
		{
			mat3<T> ret(uninitialized);
			//	const T *__restrict m1 = m;
			//	const T *__restrict m2 = other.m;
			//	T *__restrict m3 = ret.m;
			const T* m1 = m;
			const T* m2 = other.m;
			T* m3 = ret.m;
			// using this code only on release and when on the iphone
			m3[0] = m1[0] * m2[0] + m1[3] * m2[1] + m1[6] * m2[2];
			m3[1] = m1[1] * m2[0] + m1[4] * m2[1] + m1[7] * m2[2];
			m3[2] = m1[2] * m2[0] + m1[5] * m2[1] + m1[8] * m2[2];

			m3[3] = m1[0] * m2[3] + m1[3] * m2[4] + m1[6] * m2[5];
			m3[4] = m1[1] * m2[3] + m1[4] * m2[4] + m1[7] * m2[5];
			m3[5] = m1[2] * m2[3] + m1[5] * m2[4] + m1[8] * m2[5];

			m3[6] = m1[0] * m2[6] + m1[3] * m2[7] + m1[6] * m2[8];
			m3[7] = m1[1] * m2[6] + m1[4] * m2[7] + m1[7] * m2[8];
			m3[8] = m1[2] * m2[6] + m1[5] * m2[7] + m1[8] * m2[8];

			return ret;
		}

		template <typename T>
		inline mat3<T> mat3<T>::operator+(const mat3<T>& other) const
		{
			mat3<T> temp(uninitialized);
			temp[0] = m[0] + other[0];
			temp[1] = m[1] + other[1];
			temp[2] = m[2] + other[2];
			temp[3] = m[3] + other[3];
			temp[4] = m[4] + other[4];
			temp[5] = m[5] + other[5];
			temp[6] = m[6] + other[6];
			temp[7] = m[7] + other[7];
			temp[8] = m[8] + other[8];
			return temp;
		}

		template <typename T>
		inline mat3<T> mat3<T>::operator-(const mat3<T>& other) const
		{
			mat3<T> temp(uninitialized);
			temp.m[0] = m[0] - other.m[0];
			temp.m[1] = m[1] - other.m[1];
			temp.m[2] = m[2] - other.m[2];
			temp.m[3] = m[3] - other.m[3];
			temp.m[4] = m[4] - other.m[4];
			temp.m[5] = m[5] - other.m[5];
			temp.m[6] = m[6] - other.m[6];
			temp.m[7] = m[7] - other.m[7];
			temp.m[8] = m[8] - other.m[8];
			return temp;
		}

		// template <typename T>
		// inline vec2<T> mat3<T>::operator*(const vec2<T>& p) const
		// {
		// 	T px = p.x;
		// 	T py = p.y;
		// 	T x = px*m[0] + py*m[3] + m[6];
		// 	T y = px*m[1] + py*m[4] + m[7];
		// 	return vec2<T>(x, y);
		// }

		// template <typename T>
		// inline vec3<T> mat3<T>::operator*(const vec3<T>& p) const
		// {
		// 	T px = p.x;
		// 	T py = p.y;
		// 	T pz = p.z;
		// 	T x = px*m[0] + py*m[3] + m[6];
		// 	T y = px*m[1] + py*m[4] + m[7];
		// 	T z = px*m[2] + py*m[5] + m[8];
		// 	return vec3<T>(x, y, z);
		// }

		template <typename T>
		mat3<T>& mat3<T>::operator+=(const mat3<T>& other)
		{
			for (uint32_t i = 0; i < 9; i++)
			{
				m[i] += other.m[i];
			}
			return *this;
		}

		template <typename T>
		mat3<T>& mat3<T>::operator-=(const mat3<T>& other)
		{
			for (uint32_t i = 0; i < 9; i++)
			{
				m[i] -= other.m[i];
			}
			return *this;
		}


	}
}