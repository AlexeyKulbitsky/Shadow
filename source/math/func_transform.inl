namespace sh
{
	namespace math
	{

		//equivalent to m = m1 * m2
		template<typename T>
		mat3<T> multiply(const mat3<T>& m1, const mat3<T>& m2)
		{
			return m1*m2;
		}

		//multiplies only the 3x2 parts of the matrices
		template<typename T>
		mat3<T> multiply32(const mat3<T>& _m1, const mat3<T>& _m2)
		{
			mat3<T> ret(mat3<T>::uninitialized);
			const T* m1 = _m1;
			const T* m2 = _m2;
			T* m3 = ret.m;

			m3[0] = m1[0] * m2[0] + m1[3] * m2[1] + m1[6] * m2[2];
			m3[1] = m1[1] * m2[0] + m1[4] * m2[1] + m1[7] * m2[2];
			m3[2] = 0;

			m3[3] = m1[0] * m2[3] + m1[3] * m2[4] + m1[6] * m2[5];
			m3[4] = m1[1] * m2[3] + m1[4] * m2[4] + m1[7] * m2[5];
			m3[5] = 0;

			m3[6] = m1[0] * m2[6] + m1[3] * m2[7] + m1[6] * m2[8];
			m3[7] = m1[1] * m2[6] + m1[4] * m2[7] + m1[7] * m2[8];
			m3[8] = 1;

			return ret;
		}

		//equivalent to m = m1 * m2
		template<typename T>
		mat4<T> multiply(const mat4<T>& m1, const mat4<T>& m2)
		{
			return m1*m2;
		}

		//multiplies only the 4x3 parts of the matrices
		template<typename T>
		mat4<T> multiply43(const mat4<T>& _m1, const mat4<T>& _m2)
		{
			mat4<T> ret(mat4<T>::uninitialized);
			const T* m1 = _m1;
			const T* m2 = _m2;
			T* m3 = ret.m;

			m3[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
			m3[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
			m3[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
			m3[3] = 0;

			m3[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
			m3[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
			m3[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
			m3[7] = 0;

			m3[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
			m3[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
			m3[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
			m3[11] = 0;

			m3[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
			m3[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
			m3[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
			m3[15] = 1;

			return ret;
		}

		template <>
		inline mat4<float> multiply43(const mat4<float>& _m1, const mat4<float>& _m2)
		{
			mat4<float> ret(mat4<float>::uninitialized);
			//const T *__restrict m1 = m;
			//const T *__restrict m2 = other.m;
			//T *__restrict m3 = ret.m;
			const float* m1 = _m1;
			const float* m2 = _m2;
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

			m3[3] = 0;
			m3[7] = 0;
			m3[11] = 0;
			m3[15] = 1;

#else
			m3[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
			m3[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
			m3[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
			m3[3] = 0;

			m3[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
			m3[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
			m3[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
			m3[7] = 0;

			m3[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
			m3[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
			m3[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
			m3[11] = 0;

			m3[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
			m3[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
			m3[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
			m3[15] = 1;
#endif
			return ret;
		}

		//////////////////////////////////////////////////////////////////////////
		// mat2 vec2 transforms
		//////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline vec2<T> transform(const mat2<T>& m, const vec2<T>& v)
		{
			return vec2<T>(
				v.x*m[0] + v.y*m[2],
				v.x*m[1] + v.y*m[3]
				);
		}

		//////////////////////////////////////////////////////////////////////////
		// mat3 vec2 transforms
		//////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline vec2<T> transform(const mat3<T>& m, const vec2<T>& v)
		{
			return vec2<T>(
				v.x*m[0] + v.y*m[3] + m[6],
				v.x*m[1] + v.y*m[4] + m[7]
				);
		}

		//////////////////////////////////////////////////////////////////////////
		// mat4 vec4 transforms
		//////////////////////////////////////////////////////////////////////////

#ifdef WIN32
		inline vec4<float> transform(const mat4<float>& m, const vec4<float>& vect)
		{
			vec4<float> ret;
			__m128 m0 = _mm_loadu_ps(&m[0]);
			__m128 m1 = _mm_loadu_ps(&m[4]);
			__m128 m2 = _mm_loadu_ps(&m[8]);
			__m128 m3 = _mm_loadu_ps(&m[12]);

			__m128 mx = _mm_set_ps1(vect.x);
			__m128 my = _mm_set_ps1(vect.y);
			__m128 mz = _mm_set_ps1(vect.z);
			__m128 mw = _mm_set_ps1(vect.w);

			__m128 c0 = _mm_mul_ps(mx, m0);
			__m128 c1 = _mm_mul_ps(my, m1);
			__m128 c2 = _mm_mul_ps(mz, m2);
			__m128 c3 = _mm_mul_ps(mw, m3);

			__m128 t0 = _mm_add_ps(c0, c1);
			__m128 t1 = _mm_add_ps(c2, c3);
			__m128 mp = _mm_add_ps(t0, t1);
			_mm_storeu_ps((float*)&ret.x, mp);
			return ret;
		}
#endif//WIN32


		template<typename T>
		inline vec4<T> transform(const mat4<T>& m, const vec4<T>& vect)
		{
			return vec4<T>(
				vect.x*m[0] + vect.y*m[4] + vect.z*m[8] + vect.w*m[12],
				vect.x*m[1] + vect.y*m[5] + vect.z*m[9] + vect.w*m[13],
				vect.x*m[2] + vect.y*m[6] + vect.z*m[10] + vect.w*m[14],
				vect.x*m[3] + vect.y*m[7] + vect.z*m[11] + vect.w*m[15]
				);
		}

		template<typename T>
		inline vec4<T> project(const mat4<T>& m, const vec4<T>& v)
		{
			vec4<T> tmp = transform(m, v);
			if (!isZero(tmp.w))
			{
				T invW = T(1) / tmp.w;
				return vec4<T>(vec3<T>(tmp) * invW, tmp.w);
			}
			else
			{
				return tmp;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// mat3 vec3 transforms
		//////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline vec3<T> transform(const mat3<T>& m, const vec3<T>& v)
		{
			return vec3<T>(
				v.x*m[0] + v.y*m[3] + v.z*m[6],
				v.x*m[1] + v.y*m[4] + v.z*m[7],
				v.x*m[2] + v.y*m[5] + v.z*m[8]
				);
		}

		template<typename T>
		inline vec2<T> rotate(const mat3<T>& m, const vec2<T>& v)
		{
			return vec2<T>(v.x*m[0] + v.y*m[3], v.x*m[1] + v.y*m[4]);
		}

		//////////////////////////////////////////////////////////////////////////
		// mat4 vec3 transforms
		//////////////////////////////////////////////////////////////////////////

#ifdef WIN32
		inline vec3<float> transform(const mat4<float>& m, const vec3<float>& vect)
		{
			__m128 m0 = _mm_loadu_ps(&m[0]);
			__m128 m1 = _mm_loadu_ps(&m[4]);
			__m128 m2 = _mm_loadu_ps(&m[8]);
			__m128 m3 = _mm_loadu_ps(&m[12]);

			__m128 mx = _mm_set_ps1(vect.x);
			__m128 my = _mm_set_ps1(vect.y);
			__m128 mz = _mm_set_ps1(vect.z);

			__m128 c0 = _mm_mul_ps(mx, m0);
			__m128 c1 = _mm_mul_ps(my, m1);
			__m128 c2 = _mm_mul_ps(mz, m2);

			__m128 t0 = _mm_add_ps(c0, c1);
			__m128 t1 = _mm_add_ps(c2, m3);
			__m128 mp = _mm_add_ps(t0, t1);

			vec4<float> tmp;
			_mm_storeu_ps((float*)&tmp.x, mp);
			return vec3<float>(tmp.x, tmp.y, tmp.z);
		}
#endif//WIN32

		template<typename T>
		inline vec3<T> transform(const mat4<T>& m, const vec3<T>& vect)
		{
			return vec3<T>(
				vect.x*m[0] + vect.y*m[4] + vect.z*m[8] + m[12],
				vect.x*m[1] + vect.y*m[5] + vect.z*m[9] + m[13],
				vect.x*m[2] + vect.y*m[6] + vect.z*m[10] + m[14]
				);
		}

		template<typename T>
		inline vec3<T> rotate(const mat4<T>& m, const vec3<T>& vect)
		{
			return vec3<T>(
				vect.x*m[0] + vect.y*m[4] + vect.z*m[8],
				vect.x*m[1] + vect.y*m[5] + vect.z*m[9],
				vect.x*m[2] + vect.y*m[6] + vect.z*m[10]
				);
		}
		template<typename T>
		inline vec3<T> rotate(const quat<T>& q, const vec3<T>& vect)
		{
			return q * vect;
		}

		template<typename T>
		inline vec3<T> project(const mat4<T>& m, const vec3<T>& v)
		{
			vec4<T> tmp = transform(m, vec4<T>(v, T(1)));
			if (!isZero(tmp.w))
			{
				T invW = T(1) / tmp.w;
				return vec3<T>(tmp)*invW;
			}
			else
			{
				return vec3<T>(tmp);
			}
		}

		template<typename T>
		void transformArray(const mat4<T>& m, uint32_t count, const vec3<T>* input, vec3<T>* output)
		{
			for (uint32_t i = 0; i < count; ++i)
			{
				output[i] = transform(m, input[i]);
			}
		}

		template<typename T>
		void transformArray(const mat4<T>& m, uint32_t count, const vec4<T>* input, vec4<T>* output)
		{
			for (uint32_t i = 0; i < count; ++i)
			{
				output[i] = transform(m, input[i]);
			}
		}


	}
}