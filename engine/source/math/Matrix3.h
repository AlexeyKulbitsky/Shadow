#ifndef SHADOW_MATRIX3_INCLUDE
#define SHADOW_MATRIX3_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "math/Vector3.h"

namespace sh
{
	namespace math
	{
		struct SHADOW_API Matrix3
		{
			union
			{
				float m[3][3];
				float _m[9];
			};
			
			Matrix3();
			Matrix3(float value);
			Matrix3(const Matrix3& other);

			Matrix3& operator=(const Matrix3& m2);
			Matrix3 operator*(const Matrix3& m2) const;
			Matrix3 operator+(const Matrix3& m2) const;
			Matrix3 operator-(const Matrix3& m2) const;
			Matrix3 operator-() const;
			Vector3 operator*(const Vector3& v) const;

			void SetIdentity();
			Matrix3& Transpose();
			Matrix3 GetTransposed() const;
		};
	}
}

#endif
