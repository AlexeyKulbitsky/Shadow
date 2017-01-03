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
			union
			{
				T m[4][4];
				T _m[16];
			};
			
			
			
			Matrix4()
			{				
			}

			explicit Matrix4(T value)
			{
				m[0][0] = m[0][1] = m[0][2] = m[0][3] = value;
				m[1][0] = m[1][1] = m[1][2] = m[1][3] = value;
				m[2][0] = m[2][1] = m[2][2] = m[2][3] = value;
				m[3][0] = m[3][1] = m[3][2] = m[3][3] = value;

			}
			Matrix4(const Matrix4<T>& other)
			{
				operator=(other);
			}

			Matrix4(const Matrix3<T>& other)
			{
				operator=(other);
			}

			inline Matrix4(
				T m00, T m01, T m02, T m03,
				T m10, T m11, T m12, T m13,
				T m20, T m21, T m22, T m23,
				T m30, T m31, T m32, T m33 )
			{
				m[0][0] = m00;
				m[0][1] = m01;
				m[0][2] = m02;
				m[0][3] = m03;
				m[1][0] = m10;
				m[1][1] = m11;
				m[1][2] = m12;
				m[1][3] = m13;
				m[2][0] = m20;
				m[2][1] = m21;
				m[2][2] = m22;
				m[2][3] = m23;
				m[3][0] = m30;
				m[3][1] = m31;
				m[3][2] = m32;
				m[3][3] = m33;
			}

			Matrix4<T>& operator=(const Matrix4<T>& m2)
			{
				m[0][0] = m2.m[0][0];
				m[0][1] = m2.m[0][1];
				m[0][2] = m2.m[0][2];
				m[0][3] = m2.m[0][3];

				m[1][0] = m2.m[1][0];
				m[1][1] = m2.m[1][1];
				m[1][2] = m2.m[1][2];
				m[1][3] = m2.m[1][3];

				m[2][0] = m2.m[2][0];
				m[2][1] = m2.m[2][1];
				m[2][2] = m2.m[2][2];
				m[2][3] = m2.m[2][3];

				m[3][0] = m2.m[3][0];
				m[3][1] = m2.m[3][1];
				m[3][2] = m2.m[3][2];
				m[3][3] = m2.m[3][3];

				return *this;
			}

			Matrix4<T>& operator=(const Matrix3<T>& m2)
			{
				m[0][0] = m2.m[0][0];
				m[0][1] = m2.m[0][1];
				m[0][2] = m2.m[0][2];
				m[0][3] = (T)0;

				m[1][0] = m2.m[1][0];
				m[1][1] = m2.m[1][1];
				m[1][2] = m2.m[1][2];
				m[1][3] = (T)0;

				m[2][0] = m2.m[2][0];
				m[2][1] = m2.m[2][1];
				m[2][2] = m2.m[2][2];
				m[2][3] = (T)0;

				m[3][0] = (T)0;
				m[3][1] = (T)0;
				m[3][2] = (T)0;
				m[3][3] = (T)1;

				return *this;
			}

			Matrix4<T> operator*(const Matrix4<T>& m2) const
			{
				Matrix4<T> r;

				r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
				r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
				r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
				r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

				r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
				r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
				r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
				r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

				r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
				r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
				r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
				r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

				r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
				r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
				r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
				r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

				return r;
			}

			Vector3<T> operator*(const Vector3<T> &v) const
			{
				Vector3<T> r;

				T fInvW = 1.0f / (m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3]);

				r.x = (m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3]) * fInvW;
				r.y = (m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3]) * fInvW;
				r.z = (m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]) * fInvW;

				return r;
			}

			Vector4<T> operator*(const Vector4<T>& v) const
			{
				return Vector4<T>(
					m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
					m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
					m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
					m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
					);
			}
			

			Matrix4<T> operator+(const Matrix4<T>& m2) const
			{
				Matrix4<T> r;

				r.m[0][0] = m[0][0] + m2.m[0][0];
				r.m[0][1] = m[0][1] + m2.m[0][1];
				r.m[0][2] = m[0][2] + m2.m[0][2];
				r.m[0][3] = m[0][3] + m2.m[0][3];

				r.m[1][0] = m[1][0] + m2.m[1][0];
				r.m[1][1] = m[1][1] + m2.m[1][1];
				r.m[1][2] = m[1][2] + m2.m[1][2];
				r.m[1][3] = m[1][3] + m2.m[1][3];

				r.m[2][0] = m[2][0] + m2.m[2][0];
				r.m[2][1] = m[2][1] + m2.m[2][1];
				r.m[2][2] = m[2][2] + m2.m[2][2];
				r.m[2][3] = m[2][3] + m2.m[2][3];

				r.m[3][0] = m[3][0] + m2.m[3][0];
				r.m[3][1] = m[3][1] + m2.m[3][1];
				r.m[3][2] = m[3][2] + m2.m[3][2];
				r.m[3][3] = m[3][3] + m2.m[3][3];

				return r;
			}

			Matrix4<T> operator-(const Matrix4<T>& m2) const
			{
				Matrix4 r;

				r.m[0][0] = m[0][0] - m2.m[0][0];
				r.m[0][1] = m[0][1] - m2.m[0][1];
				r.m[0][2] = m[0][2] - m2.m[0][2];
				r.m[0][3] = m[0][3] - m2.m[0][3];

				r.m[1][0] = m[1][0] - m2.m[1][0];
				r.m[1][1] = m[1][1] - m2.m[1][1];
				r.m[1][2] = m[1][2] - m2.m[1][2];
				r.m[1][3] = m[1][3] - m2.m[1][3];

				r.m[2][0] = m[2][0] - m2.m[2][0];
				r.m[2][1] = m[2][1] - m2.m[2][1];
				r.m[2][2] = m[2][2] - m2.m[2][2];
				r.m[2][3] = m[2][3] - m2.m[2][3];

				r.m[3][0] = m[3][0] - m2.m[3][0];
				r.m[3][1] = m[3][1] - m2.m[3][1];
				r.m[3][2] = m[3][2] - m2.m[3][2];
				r.m[3][3] = m[3][3] - m2.m[3][3];

				return r;
			}

			void SetIdentity()
			{
				m[0][0] = (T)1; m[0][1] = (T)0; m[0][2] = (T)0; m[0][3] = (T)0;
				m[1][0] = (T)0; m[1][1] = (T)1; m[1][2] = (T)0; m[1][3] = (T)0;
				m[2][0] = (T)0; m[2][1] = (T)0; m[2][2] = (T)1; m[2][3] = (T)0;
				m[3][0] = (T)0; m[3][1] = (T)0; m[3][2] = (T)0; m[3][3] = (T)1;				
			}


			Matrix4<T>& Transpose()
			{
				std::swap(m[0][1], m[1][0]);
				std::swap(m[0][2], m[2][0]);
				std::swap(m[0][3], m[3][0]);
				std::swap(m[1][2], m[2][1]);
				std::swap(m[1][3], m[3][1]);
				std::swap(m[2][3], m[3][2]);
				return *this;
			}

			Matrix4<T> GetTransposed() const
			{
				Matrix4<T> r(*this);
				r.Transpose();
				return r;
			}

			void SetScale(const Vector3<T>& scale)
			{
				m[0][0] = scale.x;
				m[1][1] = scale.y;
				m[2][2] = scale.z;
			}

			Vector3<T> GetScale() const
			{
				return Vector3<T>(m[0][0], m[1][1], m[2][2]);
			}

			void SetTranslation(const Vector3<T>& translation)
			{
				m[0][3] = translation.x;
				m[1][3] = translation.y;
				m[2][3] = translation.z;
			}

			Vector3<T> GetTranslation() const
			{
				return Vector3<T>(m[0][3], m[1][3], m[2][3]);
			}			

			void SetPerspective(T fovy, T aspect, T zNear, T zFar)
			{
				const T tanFov = Tan(fovy / static_cast<T>(2));

				m[0][0] = 1 / (aspect * tanFov);
				m[0][1] = 0;
				m[0][2] = 0;
				m[0][3] = 0;

				m[1][0] = 0;
				m[1][1] = 1 / tanFov;
				m[1][2] = 0;
				m[1][3] = 0;

				m[2][0] = 0;
				m[2][1] = 0;
				m[2][2] = (-1) * (zFar + zNear) / (zFar - zNear);
				m[2][3] = (-1) * (2 * zFar * zNear) / (zFar - zNear);

				m[3][0] = 0;
				m[3][1] = 0;
				m[3][2] = -1;
				m[3][3] = 0;
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

			Matrix4<T> GetInversed() const
			{
				T m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
				T m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
				T m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
				T m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

				T v0 = m20 * m31 - m21 * m30;
				T v1 = m20 * m32 - m22 * m30;
				T v2 = m20 * m33 - m23 * m30;
				T v3 = m21 * m32 - m22 * m31;
				T v4 = m21 * m33 - m23 * m31;
				T v5 = m22 * m33 - m23 * m32;

				T t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
				T t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
				T t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
				T t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

				T invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

				T d00 = t00 * invDet;
				T d10 = t10 * invDet;
				T d20 = t20 * invDet;
				T d30 = t30 * invDet;

				T d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
				T d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
				T d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
				T d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

				v0 = m10 * m31 - m11 * m30;
				v1 = m10 * m32 - m12 * m30;
				v2 = m10 * m33 - m13 * m30;
				v3 = m11 * m32 - m12 * m31;
				v4 = m11 * m33 - m13 * m31;
				v5 = m12 * m33 - m13 * m32;

				T d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
				T d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
				T d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
				T d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

				v0 = m21 * m10 - m20 * m11;
				v1 = m22 * m10 - m20 * m12;
				v2 = m23 * m10 - m20 * m13;
				v3 = m22 * m11 - m21 * m12;
				v4 = m23 * m11 - m21 * m13;
				v5 = m23 * m12 - m22 * m13;

				T d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
				T d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
				T d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
				T d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

				return Matrix4(
					d00, d01, d02, d03,
					d10, d11, d12, d13,
					d20, d21, d22, d23,
					d30, d31, d32, d33);
			}
		};
	}
}

#endif
