#ifndef SHADOW_MATRIX3_INCLUDE
#define SHADOW_MATRIX3_INCLUDE

#include "Vector3.h"

namespace sh
{
	namespace math
	{
		template<typename T>
		struct Matrix3
		{
			T m[9];
			
			Matrix3()
			{
			}

			explicit Matrix3(T value)
			{
				m[0] = m[1] = m[2] = value;
				m[3] = m[4] = m[5] = value;
				m[6] = m[7] = m[8] = value;

			}
			Matrix3(const Matrix3<T>& other)
			{
				m[0] = other.m[0];
				m[1] = other.m[1];
				m[2] = other.m[2];
				m[3] = other.m[3];
				m[4] = other.m[4];
				m[5] = other.m[5];
				m[6] = other.m[6];
				m[7] = other.m[7];
				m[8] = other.m[8];
			}

			Matrix3<T>& operator=(const Matrix3<T>& other)
			{
				m[0] = other.m[0];
				m[1] = other.m[1];
				m[2] = other.m[2];
				m[3] = other.m[3];
				m[4] = other.m[4];
				m[5] = other.m[5];
				m[6] = other.m[6];
				m[7] = other.m[7];
				m[8] = other.m[8];
				return *this;
			}

			Matrix3<T> operator*(const Matrix3<T>& other) const
			{
				Matrix3<T> r;

				const T* m1 = m;
				const T* m2 = other.m;
				T* m3 = r.m;

				m3[0] = m1[0] * m2[0] + m1[3] * m2[1] + m1[6] * m2[2];
				m3[1] = m1[1] * m2[0] + m1[4] * m2[1] + m1[7] * m2[2];
				m3[2] = m1[2] * m2[0] + m1[5] * m2[1] + m1[8] * m2[2];

				m3[3] = m1[0] * m2[3] + m1[3] * m2[4] + m1[6] * m2[5];
				m3[4] = m1[1] * m2[3] + m1[4] * m2[4] + m1[7] * m2[5];
				m3[5] = m1[2] * m2[3] + m1[5] * m2[4] + m1[8] * m2[5];

				m3[6] = m1[0] * m2[6] + m1[3] * m2[7] + m1[6] * m2[8];
				m3[7] = m1[1] * m2[6] + m1[4] * m2[7] + m1[7] * m2[8];
				m3[8] = m1[2] * m2[6] + m1[5] * m2[7] + m1[8] * m2[8];

				return r;
			}

			Matrix3<T> operator+(const Matrix3<T>& other) const
			{
				Matrix3<T> r;

				r[0] = m[0] + other[0];
				r[1] = m[1] + other[1];
				r[2] = m[2] + other[2];
				r[3] = m[3] + other[3];
				r[4] = m[4] + other[4];
				r[5] = m[5] + other[5];
				r[6] = m[6] + other[6];
				r[7] = m[7] + other[7];
				r[8] = m[8] + other[8];

				return r;
			}

			Matrix3<T> operator-(const Matrix3<T>& m2) const
			{
				Matrix3<T> r;

				r.m[0] = m[0] - other.m[0];
				r.m[1] = m[1] - other.m[1];
				r.m[2] = m[2] - other.m[2];
				r.m[3] = m[3] - other.m[3];
				r.m[4] = m[4] - other.m[4];
				r.m[5] = m[5] - other.m[5];
				r.m[6] = m[6] - other.m[6];
				r.m[7] = m[7] - other.m[7];
				r.m[8] = m[8] - other.m[8];

				return r;
			}

			Matrix3<T> operator-() const
			{
				Matrix3<T> r;

				r.m[0] = -m[0];
				r.m[1] = -m[1];
				r.m[2] = -m[2];
				r.m[3] = -m[3];
				r.m[4] = -m[4];
				r.m[5] = -m[5]; 
				r.m[6] = -m[6];
				r.m[7] = -m[7];
				r.m[8] = -m[8];

				return r;
			}

			Vector3<T> operator*(const Vector3<T>& v) const
			{
				return Vector3<T>(
					m[0] * v.x + m[3] * v.y + m[6] * v.z,
					m[1] * v.x + m[4] * v.y + m[7] * v.z,
					m[2] * v.x + m[5] * v.y + m[8] * v.z
					);
			}

			void SetIdentity()
			{
				m[0] = (T)1; m[1] = (T)0; m[2] = (T)0;
				m[3] = (T)0; m[4] = (T)1; m[5] = (T)0;
				m[6] = (T)0; m[7] = (T)0; m[8] = (T)1;
			}

			Matrix3<T>& Transpose()
			{
				std::swap(m[1], m[3]);
				std::swap(m[2], m[6]);
				std::swap(m[5], m[7]);
				return *this;
			}

			Matrix3<T> GetTransposed() const
			{
				Matrix3<T> r(*this);
				r.Transpose();
				return r;
			}
		};
	}
}

#endif
