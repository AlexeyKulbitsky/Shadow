#ifndef SHADOW_QUATERNION_INCLUDE
#define SHADOW_QUATERNION_INCLUDE

#include "common/Prerequisities.h"
#include "math/Matrix4.h"
#include "math/Matrix3.h"

namespace sh
{
	namespace math
	{
		class SHADOW_API Quaternion
		{
		public:
			float x, y, z, w;

			Quaternion();
			Quaternion(float _x, float _y, float _z, float _w);
			Quaternion(float s);
			Quaternion(const Quaternion& v);
			Quaternion(const Vector3& axis, float angle);

			void SetFromAxisAngle(const Vector3& axis, float angle);
			void SetFromEulerXYZ(const Vector3& angles);
			Quaternion& operator=(const Quaternion& q);
			Quaternion operator*(const Quaternion& other) const;
			Quaternion& operator*=(float s);
			Quaternion operator/(float s) const;
						
			void Normalize();
			void RotationFromTo(const Vector3& a, const Vector3& b);
			void LookAt(const Vector3& direction, const Vector3& up);
			Matrix4 GetAsMatrix4() const;
			Matrix3 GetAsMatrix3() const;
			void GetAsEulerXYZ(Vector3& res) const;
			Quaternion& Invert();
			Quaternion GetInverted() const;
			float Norm() const;
			Vector3 operator*(const Vector3& v) const;
			Vector3 Rotate(const Vector3& v) const;
			Quaternion& SetIndentity();
		};
	}
}

#endif
