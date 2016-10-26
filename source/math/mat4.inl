namespace sh
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////
		//constructors
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat4<T>::mat4()
		{
			setIdentity();
		}

		template <typename T>
		inline mat4<T>::mat4(ZUninitialized)
		{
		}

		template <typename T>
		inline mat4<T>::mat4(T value)
		{
			m[0] = m[1] = m[2] = m[3] = value;
			m[4] = m[5] = m[6] = m[7] = value;
			m[8] = m[9] = m[10] = m[11] = value;
			m[12] = m[13] = m[14] = m[15] = value;
		}

		template <typename T>
		inline mat4<T>::mat4(const mat4<T>& other)
		{
			*this = other;
		}

		template<typename T>
		template<typename U>
		inline mat4<T>::mat4(const mat4<U>& other)
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
			m[9] = (T)other.m[9];
			m[10] = (T)other.m[10];
			m[11] = (T)other.m[11];
			m[12] = (T)other.m[12];
			m[13] = (T)other.m[13];
			m[14] = (T)other.m[14];
			m[15] = (T)other.m[15];
		}

		template<typename T>
		inline mat4<T>::mat4(const T values[16])
		{
			set(values);
		}

		///////////////////////////////////////////////////////////////////////////////
		//methods
		///////////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline mat4<T>& mat4<T>::set(const T values[16])
		{
			assert(values);
			memcpy(m, values, sizeof(T) * 16);
			return *this;
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setIdentity()
		{
			setRotationPartIdentity();
			m[15] = (T)1.0;
			m[12] = m[13] = m[14] = (T)0.0;
			return *this;
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setRotationIdentity()
		{
			MDEPRECATED("Will be removed on 1 March 2012. Please replace with setRotationPartIdentity()");
			return setRotationPartIdentity();
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setRotationPartIdentity()
		{
			m[0] = m[5] = m[10] = (T)1.0;
			m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = (T)0.0;
			return *this;
		}

		template <typename T>
		inline bool mat4<T>::invert()
		{
			double t0 = m[10] * m[15] - m[11] * m[14];
			double t1 = m[6] * m[15] - m[7] * m[14];
			double t2 = m[6] * m[11] - m[7] * m[10];
			double t3 = m[2] * m[15] - m[3] * m[14];
			double t4 = m[2] * m[11] - m[3] * m[10];
			double t5 = m[2] * m[7] - m[3] * m[6];

			double t6 = m[8] * m[13] - m[9] * m[12];
			double t7 = m[4] * m[13] - m[5] * m[12];
			double t8 = m[4] * m[9] - m[5] * m[8];
			double t9 = m[0] * m[13] - m[1] * m[12];
			double t10 = m[0] * m[9] - m[1] * m[8];
			double t11 = m[0] * m[5] - m[1] * m[4];

			double det = t0 * t11 - t1 * t10 + t2 * t9 + t3 * t8 - t4 * t7 + t5 * t6;
			if (isZero(det))
			{
				return false;
			}

			det = 1.0 / det;
			T ft0 = (T)(t0 * det);
			T ft1 = (T)(t1 * det);
			T ft2 = (T)(t2 * det);
			T ft3 = (T)(t3 * det);
			T ft4 = (T)(t4 * det);
			T ft5 = (T)(t5 * det);
			T ft6 = (T)(t6 * det);
			T ft7 = (T)(t7 * det);
			T ft8 = (T)(t8 * det);
			T ft9 = (T)(t9 * det);
			T ft10 = (T)(t10 * det);
			T ft11 = (T)(t11 * det);

			mat4<T> out;
			out.m[0] = m[5] * ft0 - m[9] * ft1 + m[13] * ft2;
			out.m[1] = m[9] * ft3 - m[1] * ft0 - m[13] * ft4;
			out.m[2] = m[1] * ft1 - m[5] * ft3 + m[13] * ft5;
			out.m[3] = m[5] * ft4 - m[1] * ft2 - m[9] * ft5;

			out.m[4] = m[8] * ft1 - m[4] * ft0 - m[12] * ft2;
			out.m[5] = m[0] * ft0 - m[8] * ft3 + m[12] * ft4;
			out.m[6] = m[4] * ft3 - m[0] * ft1 - m[12] * ft5;
			out.m[7] = m[0] * ft2 - m[4] * ft4 + m[8] * ft5;

			out.m[8] = m[7] * ft6 - m[11] * ft7 + m[15] * ft8;
			out.m[9] = m[11] * ft9 - m[3] * ft6 - m[15] * ft10;
			out.m[10] = m[3] * ft7 - m[7] * ft9 + m[15] * ft11;
			out.m[11] = m[7] * ft10 - m[3] * ft8 - m[11] * ft11;

			out.m[12] = m[10] * ft7 - m[6] * ft6 - m[14] * ft8;
			out.m[13] = m[2] * ft6 - m[10] * ft9 + m[14] * ft10;
			out.m[14] = m[6] * ft9 - m[2] * ft7 - m[14] * ft11;
			out.m[15] = m[2] * ft8 - m[6] * ft10 + m[10] * ft11;

			*this = out;
			return true;
		}

		template <typename T>
		inline mat4<T>& mat4<T>::transpose()
		{
			std::swap(m[1], m[4]);
			std::swap(m[2], m[8]);
			std::swap(m[3], m[12]);
			std::swap(m[6], m[9]);
			std::swap(m[7], m[13]);
			std::swap(m[11], m[14]);
			return *this;
		}

		template <typename T>
		inline vec4<T> mat4<T>::getRow(uint32_t row) const
		{
			assert(row < 4);
			uint32_t idx = row << 2;
			return vec4<T>(m[idx + 0], m[idx + 1], m[idx + 2], m[idx + 3]);
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setRow(uint32_t row, const vec4<T>& v)
		{
			assert(row < 4);
			uint32_t idx = row << 2;
			m[idx + 0] = v.x;
			m[idx + 1] = v.y;
			m[idx + 2] = v.z;
			m[idx + 3] = v.w;
			return *this;
		}

		template <typename T>
		inline vec4<T> mat4<T>::getColumn(uint32_t column) const
		{
			assert(column < 4);
			uint32_t idx = column;// << 2;
			return vec4<T>(m[idx + 0], m[idx + 4], m[idx + 8], m[idx + 12]);
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setColumn(uint32_t column, const vec4<T>& v)
		{
			assert(column < 4);
			uint32_t idx = column;// << 2;
			m[idx + 0] = v.x;
			m[idx + 4] = v.y;
			m[idx + 8] = v.z;
			m[idx + 12] = v.w;
			return *this;
		}

		template <typename T>
		inline vec3<T> mat4<T>::getAxisX() const
		{
			return vec3<T>(m[0], m[1], m[2]);
		}

		template <typename T>
		inline vec3<T> mat4<T>::getAxisY() const
		{
			return vec3<T>(m[4], m[5], m[6]);
		}

		template <typename T>
		inline vec3<T> mat4<T>::getAxisZ() const
		{
			return vec3<T>(m[8], m[9], m[10]);
		}

		template <typename T>
		inline const vec3<T>& mat4<T>::getTranslation() const
		{
			return *(vec3<T>*)(&m[12]);
		}

		template <typename T>
		inline vec3<T> mat4<T>::getScale() const
		{
			return vec3<T>(getAxisX().getLength(), getAxisY().getLength(), getAxisZ().getLength());
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setAxisX(const vec3<T>& axis)
		{
			m[0] = axis.x;
			m[1] = axis.y;
			m[2] = axis.z;
			return *this;
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setAxisY(const vec3<T>& axis)
		{
			m[4] = axis.x;
			m[5] = axis.y;
			m[6] = axis.z;
			return *this;
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setAxisZ(const vec3<T>& axis)
		{
			m[8] = axis.x;
			m[9] = axis.y;
			m[10] = axis.z;
			return *this;
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setTranslation(const vec3<T>& p)
		{
			m[12] = p.x;
			m[13] = p.y;
			m[14] = p.z;
			return *this;
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setScale(const vec3<T>& s)
		{
			m[0] = s.x;
			m[5] = s.y;
			m[10] = s.z;
			return *this;
		}

		template <typename T>
		inline mat3<T> mat4<T>::getRotation() const
		{
			MDEPRECATED("Will be removed on 1 March 2012. Please replace with getRotationPart()");
			return getRotationPart();
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setRotation(const mat3<T>& rot)
		{
			MDEPRECATED("Will be removed on 1 March 2012. Please replace with setRotationPart()");
			return setRotationPart(rot);
		}

		template <typename T>
		inline mat3<T> mat4<T>::getRotationPart() const
		{
			return mat3<T>(*this);
		}

		template <typename T>
		inline mat4<T>& mat4<T>::setRotationPart(const mat3<T>& rot)
		{
			m[0] = rot[0];
			m[1] = rot[1];
			m[2] = rot[2];

			m[4] = rot[3];
			m[5] = rot[4];
			m[6] = rot[5];

			m[8] = rot[6];
			m[9] = rot[7];
			m[10] = rot[8];
			return *this;
		}


		//////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline bool mat4<T>::operator==(const mat4<T>& m) const
		{
			return memcmp(this->m, m.m, sizeof(T) * 16) == 0;
		}
		template <typename T>
		inline bool mat4<T>::operator!=(const mat4<T>& m) const
		{
			return !operator==(m);
		}

		template <typename T>
		inline mat4<T>& mat4<T>::postScale(const vec3<T>& scale)
		{
			m[0] *= scale.x;
			m[1] *= scale.x;
			m[2] *= scale.x;

			m[4] *= scale.y;
			m[5] *= scale.y;
			m[6] *= scale.y;

			m[8] *= scale.z;
			m[9] *= scale.z;
			m[10] *= scale.z;
			return *this;
		}

		template <typename T>
		inline mat4<T>& mat4<T>::postTranslate(const vec3<T>& trans)
		{
			m[12] += m[0] * trans.x + m[4] * trans.y + m[8] * trans.z;
			m[13] += m[1] * trans.x + m[5] * trans.y + m[9] * trans.z;
			m[14] += m[2] * trans.x + m[6] * trans.y + m[10] * trans.z;
			return *this;
		}

		///////////////////////////////////////////////////////////////////////////////
		// indexing operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat4<T>::operator T*()
		{
			return m;
		}
		template <typename T>
		inline mat4<T>::operator const T*() const
		{
			return m;
		}

		template <typename T>
		inline T& mat4<T>::operator()(uint32_t row, uint32_t col)
		{
			assert(row * 4 + col < 16);
			return m[row * 4 + col];
		}

		template <typename T>
		inline const T& mat4<T>::operator()(uint32_t row, uint32_t col) const
		{
			assert(row * 4 + col < 16);
			return m[row * 4 + col];
		}

		///////////////////////////////////////////////////////////////////////////////
		// scalar operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat4<T> mat4<T>::operator*(T scalar) const
		{
			mat4<T> temp(uninitialized);
			temp.m[0] = m[0] * scalar;
			temp.m[1] = m[1] * scalar;
			temp.m[2] = m[2] * scalar;
			temp.m[3] = m[3] * scalar;
			temp.m[4] = m[4] * scalar;
			temp.m[5] = m[5] * scalar;
			temp.m[6] = m[6] * scalar;
			temp.m[7] = m[7] * scalar;
			temp.m[8] = m[8] * scalar;
			temp.m[9] = m[9] * scalar;
			temp.m[10] = m[10] * scalar;
			temp.m[11] = m[11] * scalar;
			temp.m[12] = m[12] * scalar;
			temp.m[13] = m[13] * scalar;
			temp.m[14] = m[14] * scalar;
			temp.m[15] = m[15] * scalar;
			return temp;
		}

		template <>
		inline mat4<float> mat4<float>::operator*(float scalar) const
		{
			mat4<float> temp(uninitialized);
#if defined(MATH_USE_NEON)
			asm volatile
				(
					"vldmia %0, { q0-q3 }	\n\t"
					"vdup.f32 q4, %1    	\n\t" // vdup splates a scalar into a vector

					"vmul.f32 q0, q0, q4	\n\t"
					"vmul.f32 q1, q1, q4	\n\t"
					"vmul.f32 q2, q2, q4	\n\t"
					"vmul.f32 q3, q3, q4	\n\t"

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
			temp.m[9] = m[9] * scalar;
			temp.m[10] = m[10] * scalar;
			temp.m[11] = m[11] * scalar;
			temp.m[12] = m[12] * scalar;
			temp.m[13] = m[13] * scalar;
			temp.m[14] = m[14] * scalar;
			temp.m[15] = m[15] * scalar;
#endif
			return temp;
		}

		template <typename T>
		inline mat4<T>& mat4<T>::operator*=(T scalar)
		{
			m[0] *= scalar;
			m[1] *= scalar;
			m[2] *= scalar;
			m[3] *= scalar;
			m[4] *= scalar;
			m[5] *= scalar;
			m[6] *= scalar;
			m[7] *= scalar;
			m[8] *= scalar;
			m[9] *= scalar;
			m[10] *= scalar;
			m[11] *= scalar;
			m[12] *= scalar;
			m[13] *= scalar;
			m[14] *= scalar;
			m[15] *= scalar;
			return *this;
		}

		template <>
		inline mat4<float>& mat4<float>::operator*=(float scalar)
		{
#if defined(MATH_USE_NEON)
			asm volatile
				(
					"vldmia %0, { q0-q3 }	\n\t"
					"vdup.f32 q4, %1    	\n\t" // vdup splates a scalar into a vector

					"vmul.f32 q0, q0, q4	\n\t"
					"vmul.f32 q1, q1, q4	\n\t"
					"vmul.f32 q2, q2, q4	\n\t"
					"vmul.f32 q3, q3, q4	\n\t"

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
			m[9] *= scalar;
			m[10] *= scalar;
			m[11] *= scalar;
			m[12] *= scalar;
			m[13] *= scalar;
			m[14] *= scalar;
			m[15] *= scalar;
#endif
			return *this;
		}

		///////////////////////////////////////////////////////////////////////////////
		// arithmetic operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline mat4<T> mat4<T>::operator*(const mat4<T>& other) const
		{
			mat4<T> ret(uninitialized);
			//const T *__restrict m1 = m;
			//const T *__restrict m2 = other.m;
			//T *__restrict m3 = ret.m;
			const T* m1 = m;
			const T* m2 = other.m;
			T* m3 = ret.m;

			// using this code only on release and when on the iphone
			m3[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
			m3[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
			m3[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
			m3[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

			m3[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
			m3[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
			m3[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
			m3[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

			m3[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
			m3[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
			m3[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
			m3[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

			m3[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
			m3[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
			m3[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
			m3[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

			return ret;
		}

		template <>
		inline mat4<float> mat4<float>::operator*(const mat4<float>& other) const
		{
			mat4<float> ret(uninitialized);
			//const T *__restrict m1 = m;
			//const T *__restrict m2 = other.m;
			//T *__restrict m3 = ret.m;
			const float* m1 = m;
			const float* m2 = other.m;
			float* m3 = ret.m;

#if defined(MATH_USE_NEON)
			asm volatile
				(
					"vldmia %1, { q8-q11 }		\n\t"	// Q8-Q11 = rows of m1
					"vldmia %2, { q4-q7 }		\n\t"	// Q4-Q7  = rows of m2

					"vmul.f32 q0, q8 , d8[0]	\n\t"
					"vmul.f32 q1, q8 , d10[0]	\n\t"
					"vmul.f32 q2, q8 , d12[0]	\n\t"
					"vmul.f32 q3, q8 , d14[0]	\n\t"

					"vmla.f32 q0, q9 , d8[1]	\n\t"
					"vmla.f32 q1, q9 , d10[1]	\n\t"
					"vmla.f32 q2, q9 , d12[1]	\n\t"
					"vmla.f32 q3, q9 , d14[1]	\n\t"

					"vmla.f32 q0, q10, d9[0]	\n\t"
					"vmla.f32 q1, q10, d11[0]	\n\t"
					"vmla.f32 q2, q10, d13[0]	\n\t"
					"vmla.f32 q3, q10, d15[0]	\n\t"

					"vmla.f32 q0, q11, d9[1]	\n\t"
					"vmla.f32 q1, q11, d11[1]	\n\t"
					"vmla.f32 q2, q11, d13[1]	\n\t"
					"vmla.f32 q3, q11, d15[1]	\n\t"

					// output = result registers
					"vstmia %0, { q0-q3 }\n\t"
					: // no output
			: "r" (m3), "r" (m1), "r" (m2)       // input
				: "memory",
				"q0", "q1", "q2", "q3",
				"q4", "q5", "q6", "q7",
				"q8", "q9", "q10", "q11"   // clobber
				);
#else
			// using this code only on release and when on the iphone
			m3[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
			m3[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
			m3[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
			m3[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

			m3[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
			m3[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
			m3[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
			m3[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

			m3[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
			m3[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
			m3[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
			m3[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

			m3[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
			m3[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
			m3[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
			m3[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
#endif
			return ret;
		}

		template <typename T>
		inline mat4<T> mat4<T>::operator+(const mat4<T>& other) const
		{
			mat4<T> temp(uninitialized);
			temp[0] = m[0] + other[0];
			temp[1] = m[1] + other[1];
			temp[2] = m[2] + other[2];
			temp[3] = m[3] + other[3];
			temp[4] = m[4] + other[4];
			temp[5] = m[5] + other[5];
			temp[6] = m[6] + other[6];
			temp[7] = m[7] + other[7];
			temp[8] = m[8] + other[8];
			temp[9] = m[9] + other[9];
			temp[10] = m[10] + other[10];
			temp[11] = m[11] + other[11];
			temp[12] = m[12] + other[12];
			temp[13] = m[13] + other[13];
			temp[14] = m[14] + other[14];
			temp[15] = m[15] + other[15];
			return temp;
		}


		template <>
		inline mat4<float> mat4<float>::operator+(const mat4<float>& other) const
		{
			mat4<float> temp(uninitialized);
#if defined(MATH_USE_NEON)
			asm volatile
				(
					"vldmia %0, { q0-q3 }	\n\t"
					"vldmia %1, { q4-q7  }	\n\t"

					"vadd.f32 q0, q0, q4	\n\t"
					"vadd.f32 q1, q1, q5	\n\t"
					"vadd.f32 q2, q2, q6	\n\t"
					"vadd.f32 q3, q3, q7	\n\t"

					"vstmia %2, { q0-q3  }	\n\t"
					: // no output
			: "r" (m), "r" (other.m), "r" (temp.m)       // input
				: "memory",
				"q0", "q1", "q2", "q3",
				"q4", "q5", "q6", "q7"
				);
#else
			temp[0] = m[0] + other[0];
			temp[1] = m[1] + other[1];
			temp[2] = m[2] + other[2];
			temp[3] = m[3] + other[3];
			temp[4] = m[4] + other[4];
			temp[5] = m[5] + other[5];
			temp[6] = m[6] + other[6];
			temp[7] = m[7] + other[7];
			temp[8] = m[8] + other[8];
			temp[9] = m[9] + other[9];
			temp[10] = m[10] + other[10];
			temp[11] = m[11] + other[11];
			temp[12] = m[12] + other[12];
			temp[13] = m[13] + other[13];
			temp[14] = m[14] + other[14];
			temp[15] = m[15] + other[15];
#endif
			return temp;
		}
		template <typename T>
		inline mat4<T> mat4<T>::operator-(const mat4<T>& other) const
		{
			mat4<T> temp(uninitialized);
			temp.m[0] = m[0] - other.m[0];
			temp.m[1] = m[1] - other.m[1];
			temp.m[2] = m[2] - other.m[2];
			temp.m[3] = m[3] - other.m[3];
			temp.m[4] = m[4] - other.m[4];
			temp.m[5] = m[5] - other.m[5];
			temp.m[6] = m[6] - other.m[6];
			temp.m[7] = m[7] - other.m[7];
			temp.m[8] = m[8] - other.m[8];
			temp.m[9] = m[9] - other.m[9];
			temp.m[10] = m[10] - other.m[10];
			temp.m[11] = m[11] - other.m[11];
			temp.m[12] = m[12] - other.m[12];
			temp.m[13] = m[13] - other.m[13];
			temp.m[14] = m[14] - other.m[14];
			temp.m[15] = m[15] - other.m[15];
			return temp;
		}

		template <>
		inline mat4<float> mat4<float>::operator-(const mat4<float>& other) const
		{
			mat4<float> temp(uninitialized);
#if defined(MATH_USE_NEON)
			asm volatile
				(
					"vldmia %0, { q0-q3 }	\n\t"
					"vldmia %1, { q4-q7  }	\n\t"

					"vsub.f32 q0, q0, q4	\n\t"
					"vsub.f32 q1, q1, q5	\n\t"
					"vsub.f32 q2, q2, q6	\n\t"
					"vsub.f32 q3, q3, q7	\n\t"

					"vstmia %2, { q0-q3  }	\n\t"
					: // no output
			: "r" (m), "r" (other.m), "r" (temp.m)       // input
				: "memory",
				"q0", "q1", "q2", "q3",
				"q4", "q5", "q6", "q7"
				);
#else
			temp.m[0] = m[0] - other.m[0];
			temp.m[1] = m[1] - other.m[1];
			temp.m[2] = m[2] - other.m[2];
			temp.m[3] = m[3] - other.m[3];
			temp.m[4] = m[4] - other.m[4];
			temp.m[5] = m[5] - other.m[5];
			temp.m[6] = m[6] - other.m[6];
			temp.m[7] = m[7] - other.m[7];
			temp.m[8] = m[8] - other.m[8];
			temp.m[9] = m[9] - other.m[9];
			temp.m[10] = m[10] - other.m[10];
			temp.m[11] = m[11] - other.m[11];
			temp.m[12] = m[12] - other.m[12];
			temp.m[13] = m[13] - other.m[13];
			temp.m[14] = m[14] - other.m[14];
			temp.m[15] = m[15] - other.m[15];
#endif
			return temp;
		}

		template <typename T>
		inline T mat4<T>::dotColumn(uint32_t column, const vec3<T>& p) const
		{
			return p.x*m[column] + p.y*m[column + 4] + p.z*m[column + 8] + m[column + 12];
		}

		template <typename T>
		mat4<T>& mat4<T>::operator+=(const mat4<T>& other)
		{
			*this = *this + other;
			return *this;
		}

		template <typename T>
		mat4<T>& mat4<T>::operator-=(const mat4<T>& other)
		{
			*this = *this - other;
			return *this;
		}

	}
}