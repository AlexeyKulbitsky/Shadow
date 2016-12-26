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
			union
			{
				T m[3][3];
				T _m[9];
			};
			
			Matrix3()
			{
			}

			explicit Matrix3(T value)
			{
				m[0][0] = m[0][1] = m[0][2] = value;
				m[1][0] = m[1][1] = m[1][2] = value;
				m[2][0] = m[2][1] = m[2][2] = value;
				

			}
			Matrix3(const Matrix3<T>& other)
			{
				operator=(other);
			}

			Matrix3<T>& operator=(const Matrix3<T>& m2)
			{
				m[0][0] = m2.m[0][0];
				m[0][1] = m2.m[0][1];
				m[0][2] = m2.m[0][2];

				m[1][0] = m2.m[1][0];
				m[1][1] = m2.m[1][1];
				m[1][2] = m2.m[1][2];

				m[2][0] = m2.m[2][0];
				m[2][1] = m2.m[2][1];
				m[2][2] = m2.m[2][2];
				return *this;
			}

			Matrix3<T> operator*(const Matrix3<T>& m2) const
			{
				Matrix3<T> r;

				r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0];
				r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1];
				r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2];

				r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0];
				r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1];
				r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2];

				r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0];
				r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1];
				r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2];

				return r;
			}

			Matrix3<T> operator+(const Matrix3<T>& m2) const
			{
				Matrix3<T> r;

				r.m[0][0] = m[0][0] + m2.m[0][0];
				r.m[0][1] = m[0][1] + m2.m[0][1];
				r.m[0][2] = m[0][2] + m2.m[0][2];

				r.m[1][0] = m[1][0] + m2.m[1][0];
				r.m[1][1] = m[1][1] + m2.m[1][1];
				r.m[1][2] = m[1][2] + m2.m[1][2];

				r.m[2][0] = m[2][0] + m2.m[2][0];
				r.m[2][1] = m[2][1] + m2.m[2][1];
				r.m[2][2] = m[2][2] + m2.m[2][2];

				return r;
			}

			Matrix3<T> operator-(const Matrix3<T>& m2) const
			{
				Matrix3<T> r;
				r.m[0][0] = m[0][0] - m2.m[0][0];
				r.m[0][1] = m[0][1] - m2.m[0][1];
				r.m[0][2] = m[0][2] - m2.m[0][2];

				r.m[1][0] = m[1][0] - m2.m[1][0];
				r.m[1][1] = m[1][1] - m2.m[1][1];
				r.m[1][2] = m[1][2] - m2.m[1][2];

				r.m[2][0] = m[2][0] - m2.m[2][0];
				r.m[2][1] = m[2][1] - m2.m[2][1];
				r.m[2][2] = m[2][2] - m2.m[2][2];

				return r;
			}

			Matrix3<T> operator-() const
			{
				Matrix3<T> r;

				r.m[0][0] = -m[0][0];
				r.m[0][1] = -m[0][1];
				r.m[0][2] = -m[0][2];

				r.m[1][0] = -m[1][0];
				r.m[1][1] = -m[1][1];
				r.m[1][2] = -m[1][2];

				r.m[2][0] = -m[2][0];
				r.m[2][1] = -m[2][1];
				r.m[2][2] = -m[2][2];

				return r;
			}

			Vector3<T> operator*(const Vector3<T>& v) const
			{
				return Vector3<T>(
					m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
					m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
					m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
					);
			}

			void SetIdentity()
			{
				m[0][0] = (T)1; m[0][1] = (T)0; m[0][2] = (T)0;
				m[1][0] = (T)0; m[1][1] = (T)1; m[1][2] = (T)0;
				m[2][0] = (T)0; m[2][1] = (T)0; m[2][2] = (T)1;
			}

			Matrix3<T>& Transpose()
			{
				std::swap(m[0][1], m[1][0]);
				std::swap(m[0][2], m[2][0]);
				std::swap(m[1][2], m[2][1]);
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
