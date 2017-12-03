#ifndef SHADOW_MATRIX4_INCLUDE
#define SHADOW_MATRIX4_INCLUDE

#include "Vector4.h"
#include "Matrix3.h"

#include "../CompileConfig.h"

namespace sh
{
	namespace math
	{
		// Column major order for OpenGL: m[column][row]
		struct SHADOW_API Matrix4
		{
			union
			{
				float m[4][4];
				float _m[16];
			};
			
			Matrix4();
			Matrix4(float value);
			Matrix4(const Matrix4& other);
			Matrix4(const Matrix3& other);
			Matrix4(
				float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33);

			static const Matrix4& Identity();

			Matrix4& operator=(const Matrix4& m2);
			Matrix4& operator=(const Matrix3& m2);
			Matrix4 operator*(const Matrix4& mat) const;
			Vector3 operator*(const Vector3& v) const;
			Vector4 operator*(const Vector4& v) const;
			Matrix4 operator+(const Matrix4& m2) const;
			Matrix4 operator-(const Matrix4& m2) const;

			void SetIdentity();

			Matrix4& Transpose();
			Matrix4 GetTransposed() const;
			void SetScale(const Vector3& scale);
			Vector3 GetScale() const;
			void SetTranslation(const Vector3& translation);
			Vector3 GetTranslation() const;
			void SetPerspective(float fovy, float aspect, float near, float far);
			void SetOrtho(float left, float right, float bottom, float top, float near, float far);
			Matrix4 GetInversed() const;
		};
	}
}

#endif
