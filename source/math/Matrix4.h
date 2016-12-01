#ifndef SHADOW_MATRIX4_INCLUDE
#define SHADOW_MATRIX4_INCLUDE

#include "Vector4.h"
#include "Matrix3.h"

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
				operator=(other);
			}

			Matrix4(const Matrix3<T>& other)
			{
				operator=(other);
			}

			Matrix4<T>& operator=(const Matrix4<T>& other)
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
				m[9] = other.m[9];
				m[10] = other.m[10];
				m[11] = other.m[11];

				m[12] = other.m[12];
				m[13] = other.m[13];
				m[14] = other.m[14];
				m[15] = other.m[15];

				return *this;
			}

			Matrix4<T>& operator=(const Matrix3<T>& other)
			{
				m[0] = other.m[0];
				m[1] = other.m[1];
				m[2] = other.m[2];
				m[3] = (T)0;

				m[4] = other.m[3];
				m[5] = other.m[4];
				m[6] = other.m[5];
				m[7] = (T)0;

				m[8] = other.m[6];
				m[9] = other.m[7];
				m[10] = other.m[8];
				m[11] = (T)0;

				m[12] = (T)0;
				m[13] = (T)0;
				m[14] = (T)0;
				m[15] = (T)1;

				return *this;
			}

			Matrix4<T> operator*(const Matrix4<T>& other) const
			{
				Matrix4<T> r;

				const T* m1 = m;
				const T* m2 = other.m;
				T* m3 = r.m;

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
				
				return r;
			}
			

			Matrix4<T> operator+(const Matrix4<T>& other) const
			{
				Matrix4<T> r;

				r[0] = m[0] + other[0];
				r[1] = m[1] + other[1];
				r[2] = m[2] + other[2];
				r[3] = m[3] + other[3];
				r[4] = m[4] + other[4];
				r[5] = m[5] + other[5];
				r[6] = m[6] + other[6];
				r[7] = m[7] + other[7];
				r[8] = m[8] + other[8];
				r[9] = m[9] + other[9];
				r[10] = m[10] + other[10];
				r[11] = m[11] + other[11];
				r[12] = m[12] + other[12];
				r[13] = m[13] + other[13];
				r[14] = m[14] + other[14];
				r[15] = m[15] + other[15];

				return r;
			}

			Matrix4<T> operator-(const Matrix4<T>& other) const
			{
				Matrix4 r;
				r[0] = m[0] - other[0];
				r[1] = m[1] - other[1];
				r[2] = m[2] - other[2];
				r[3] = m[3] - other[3];
				r[4] = m[4] - other[4];
				r[5] = m[5] - other[5];
				r[6] = m[6] - other[6];
				r[7] = m[7] - other[7];
				r[8] = m[8] - other[8];
				r[9] = m[9] - other[9];
				r[10] = m[10] - other[10];
				r[11] = m[11] - other[11];
				r[12] = m[12] - other[12];
				r[13] = m[13] - other[13];
				r[14] = m[14] - other[14];
				r[15] = m[15] - other[15];

				return r;
			}

			void SetIdentity()
			{
				m[0] = (T)1;
				m[1] = (T)0;
				m[2] = (T)0;
				m[3] = (T)0;

				m[4] = (T)0;
				m[5] = (T)1;
				m[6] = (T)0;
				m[7] = (T)0;

				m[8] = (T)0;
				m[9] = (T)0;
				m[10] = (T)1;
				m[11] = (T)0;

				m[12] = (T)0;
				m[13] = (T)0;
				m[14] = (T)0;
				m[15] = (T)1;
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
				Matrix4<T> r(*this);
				r.Transpose();
				return res;
			}

			void SetScale(const Vector3<T>& scale)
			{
				m[0] = scale.x;
				m[5] = scale.y;
				m[10] = scale.z;
			}

			Vector3<T> GetScale() const
			{
				return Vector3<T>(m[0], m[5], m[10]);
			}

			void SetTranslation(const Vector3<T>& translation)
			{
				m[12] = translation.x;
				m[13] = translation.y;
				m[14] = translation.z;
			}

			Vector3<T> GetTranslation() const
			{
				return Vector3<T>(m[12], m[13], m[14]);
			}			

			void SetPerspective(T fovy, T aspect, T zNear, T zFar)
			{
				const T tanFov = Tan(fovy / static_cast<T>(2));

				m[0] = 1 / (aspect * tanFov);
				m[1] = 0;
				m[2] = 0;
				m[3] = 0;

				m[4] = 0;
				m[5] = 1 / tanFov;
				m[6] = 0;
				m[7] = 0;

				m[8] = 0;
				m[9] = 0;
				m[10] = -(zFar + zNear) / (zFar - zNear);
				m[11] = -1;

				m[12] = 0;
				m[13] = 0;
				m[14] = -(2 * zFar*zNear) / (zFar - zNear);
				m[15] = 0;
			}

			/*
			void SetOrtho(T leftP, T rightP, T bottomP, T topP, T nearP, T farP)
			{
				m.m[0] = 2 / (rightP - leftP);
				m.m[1] = 0;
				m.m[2] = 0;
				m.m[3] = (-1) * (rightP + leftP) / (rightP - leftP);

				m.m[4] = 0;
				m.m[5] = 2 / (topP - bottomP);
				m.m[6] = 0;
				m.m[7] = (-1) * (topP + bottomP) / (topP - bottomP);

				m.m[8] = 0;
				m.m[9] = 0;
				m.m[10] = -2 / (farP - nearP);
				m.m[11] = (-1) * (farP + nearP) / (farP - nearP);

				m.m[12] = 0;
				m.m[13] = 0;
				m.m[14] = 0;
				m.m[15] = 1;
			}
			*/
		};
	}
}

#endif
