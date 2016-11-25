#ifndef SHADOW_MATRIX4_INCLUDE
#define SHADOW_MATRIX4_INCLUDE

#include "Vector4.h"

namespace sh
{
	namespace math
	{
		template<typename T>
		struct Matrix4
		{
			T m[16];

			Matrix4()
			{
			}

			explicit Matrix4(T value)
			{
				m[0] = m[1] = m[2] = m[3] = value;
				m[4] = m[5] = m[6] = m[7] = value;
				m[8] = m[9] = m[10] = m[11] = value;
				m[12] = m[13] = m[14] = m[15] = value;

			}
			Matrix4(const Matrix4<T>& other)
			{
				m[0] = other.m[0]; m[1] = other.m[1]; m[2] = other.m[2]; m[3] = other.m[3];
				m[4] = other.m[4]; m[5] = other.m[5]; m[6] = other.m[6]; m[7] = other.m[7];
				m[8] = other.m[8]; m[9] = other.m[9]; m[10] = other.m[10]; m[11] = other.m[11];
				m[12] = other.m[12]; m[13] = other.m[13]; m[14] = other.m[14]; m[15] = other.m[15];
			}

			Matrix4<T>& operator=(const Matrix4<T>& other)
			{
				m[0] = other.m[0]; m[1] = other.m[1]; m[2] = other.m[2]; m[3] = other.m[3];
				m[4] = other.m[4]; m[5] = other.m[5]; m[6] = other.m[6]; m[7] = other.m[7];
				m[8] = other.m[8]; m[9] = other.m[9]; m[10] = other.m[10]; m[11] = other.m[11];
				m[12] = other.m[12]; m[13] = other.m[13]; m[14] = other.m[14]; m[15] = other.m[15];
				return *this;
			}

			Matrix4<T> operator*(const Matrix4<T>& other) const
			{
				Matrix4<T> result;

				result.m[0] = m[0] * other.m[0] + m[4] * other.m[1] + m[8] * other.m[2] + m[12] * other.m[3];
				result.m[1] = m[1] * other.m[0] + m[5] * other.m[1] + m[9] * other.m[2] + m[13] * other.m[3];
				result.m[2] = m[2] * other.m[0] + m[6] * other.m[1] + m[10] * other.m[2] + m[14] * other.m[3];
				result.m[3] = m[3] * other.m[0] + m[7] * other.m[1] + m[11] * other.m[2] + m[15] * other.m[3];

				result.m[4] = m[0] * other.m[4] + m[4] * other.m[5] + m[8] * other.m[6] + m[12] * other.m[7];
				result.m[5] = m[1] * other.m[4] + m[5] * other.m[5] + m[9] * other.m[6] + m[13] * other.m[7];
				result.m[6] = m[2] * other.m[4] + m[6] * other.m[5] + m[10] * other.m[6] + m[14] * other.m[7];
				result.m[7] = m[3] * other.m[4] + m[7] * other.m[5] + m[11] * other.m[6] + m[15] * other.m[7];

				result.m[8] = m[0] * other.m[8] + m[4] * other.m[9] + m[8] * other.m[10] + m[12] * other.m[11];
				result.m[9] = m[1] * other.m[8] + m[5] * other.m[9] + m[9] * other.m[10] + m[13] * other.m[11];
				result.m[10] = m[2] * other.m[8] + m[6] * other.m[9] + m[10] * other.m[10] + m[14] * other.m[11];
				result.m[11] = m[3] * other.m[8] + m[7] * other.m[9] + m[11] * other.m[10] + m[15] * other.m[11];

				result.m[12] = m[0] * other.m[12] + m[4] * other.m[13] + m[8] * other.m[14] + m[12] * other.m[15];
				result.m[13] = m[1] * other.m[12] + m[5] * other.m[13] + m[9] * other.m[14] + m[13] * other.m[15];
				result.m[14] = m[2] * other.m[12] + m[6] * other.m[13] + m[10] * other.m[14] + m[14] * other.m[15];
				result.m[15] = m[3] * other.m[12] + m[7] * other.m[13] + m[11] * other.m[14] + m[15] * other.m[15];

				return result;
			}

			Matrix4<T>& operator*=(const Matrix4<T>& other)
			{
				m[0] = m[0] * other.m[0] + m[4] * other.m[1] + m[8] * other.m[2] + m[12] * other.m[3];
				m[1] = m[1] * other.m[0] + m[5] * other.m[1] + m[9] * other.m[2] + m[13] * other.m[3];
				m[2] = m[2] * other.m[0] + m[6] * other.m[1] + m[10] * other.m[2] + m[14] * other.m[3];
				m[3] = m[3] * other.m[0] + m[7] * other.m[1] + m[11] * other.m[2] + m[15] * other.m[3];

				m[4] = m[0] * other.m[4] + m[4] * other.m[5] + m[8] * other.m[6] + m[12] * other.m[7];
				m[5] = m[1] * other.m[4] + m[5] * other.m[5] + m[9] * other.m[6] + m[13] * other.m[7];
				m[6] = m[2] * other.m[4] + m[6] * other.m[5] + m[10] * other.m[6] + m[14] * other.m[7];
				m[7] = m[3] * other.m[4] + m[7] * other.m[5] + m[11] * other.m[6] + m[15] * other.m[7];

				m[8] = m[0] * other.m[8] + m[4] * other.m[9] + m[8] * other.m[10] + m[12] * other.m[11];
				m[9] = m[1] * other.m[8] + m[5] * other.m[9] + m[9] * other.m[10] + m[13] * other.m[11];
				m[10] = m[2] * other.m[8] + m[6] * other.m[9] + m[10] * other.m[10] + m[14] * other.m[11];
				m[11] = m[3] * other.m[8] + m[7] * other.m[9] + m[11] * other.m[10] + m[15] * other.m[11];

				m[12] = m[0] * other.m[12] + m[4] * other.m[13] + m[8] * other.m[14] + m[12] * other.m[15];
				m[13] = m[1] * other.m[12] + m[5] * other.m[13] + m[9] * other.m[14] + m[13] * other.m[15];
				m[14] = m[2] * other.m[12] + m[6] * other.m[13] + m[10] * other.m[14] + m[14] * other.m[15];
				m[15] = m[3] * other.m[12] + m[7] * other.m[13] + m[11] * other.m[14] + m[15] * other.m[15];

				return *this;
			}

			Vector4<T> operator*(const Vector4<T>& v)
			{
				return Vector4<T>
					(
						m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w,
						m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w,
						m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w,
						m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w
					);
			}

			Matrix4<T> operator+(const Matrix4<T>& other) const
			{
				Matrix4<T> result;

				result.m[0] = m[0] + other[0];
				result.m[1] = m[1] + other[1];
				result.m[2] = m[2] + other[2];
				result.m[3] = m[3] + other[3];
				result.m[4] = m[4] + other[4];
				result.m[5] = m[5] + other[5];
				result.m[6] = m[6] + other[6];
				result.m[7] = m[7] + other[7];
				result.m[8] = m[8] + other[8];
				result.m[9] = m[9] + other[9];
				result.m[10] = m[10] + other[10];
				result.m[11] = m[11] + other[11];
				result.m[12] = m[12] + other[12];
				result.m[13] = m[13] + other[13];
				result.m[14] = m[14] + other[14];
				result.m[15] = m[15] + other[15];

				return result;
			}

			Matrix4<T> operator-(const Matrix4<T>& other) const
			{
				Matrix4<T> result;
				result.m[0] = m[0] - other[0];
				result.m[1] = m[1] - other[1];
				result.m[2] = m[2] - other[2];
				result.m[3] = m[3] - other[3];
				result.m[4] = m[4] - other[4];
				result.m[5] = m[5] - other[5];
				result.m[6] = m[6] - other[6];
				result.m[7] = m[7] - other[7];
				result.m[8] = m[8] - other[8];
				result.m[9] = m[9] - other[9];
				result.m[10] = m[10] - other[10];
				result.m[11] = m[11] - other[11];
				result.m[12] = m[12] - other[12];
				result.m[13] = m[13] - other[13];
				result.m[14] = m[14] - other[14];
				result.m[15] = m[15] - other[15];
				return result;
			}

			void SetIdentity()
			{
				m[0] = (T)1; m[1] = (T)0; m[2] = (T)0; m[3] = (T)0;
				m[4] = (T)0; m[5] = (T)1; m[6] = (T)0; m[7] = (T)0;
				m[8] = (T)0; m[9] = (T)0; m[10] = (T)1; m[11] = (T)0;
				m[12] = (T)0; m[13] = (T)0; m[14] = (T)0; m[15] = (T)1;
			}

			Matrix4<T>& Transpose()
			{
				std::swap(m[1], m[4]);
				std::swap(m[2], m[8]);
				std::swap(m[3], m[12]);
				std::swap(m[6], m[9]);
				std::swap(m[7], m[13]);
				std::swap(m[11], m[14]);
				return *this;
			}

			Matrix4<T> GetTransposed() const
			{
				Matrix4<T> result;

				result.m[0] = m[0]; result.m[1] = m[4]; result.m[2] = m[8]; result.m[3] = m[12];
				result.m[4] = m[1]; result.m[5] = m[5]; result.m[6] = m[9]; result.m[7] = m[13];
				result.m[8] = m[2]; result.m[9] = m[6]; result.m[10] = m[10]; result.m[11] = m[14];
				result.m[12] = m[3]; result.m[13] = m[7]; result.m[14] = m[11]; result.m[15] = m[15];

				return result;
			}

			void SetScale(const Vector3<T>& scale)
			{
				m[0] = scale.x;
				m[5] = scale.y;
				m[10] = scale.z;
			}
		};
	}
}

#endif
