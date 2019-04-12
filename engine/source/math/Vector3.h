#ifndef SHADOW_VECTOR3_INCLUDE
#define SHADOW_VECTOR3_INCLUDE

#include "common/Prerequisities.h"
#include "math/Vector2.h"

namespace sh
{

	namespace math
	{
		class SHADOW_API Vector3
		{
		public:
			float x, y, z;

			// Constructors
			Vector3();
			Vector3(float _x, float _y, float _z);
			Vector3(float s);
			Vector3(const Vector3& v);
			Vector3(const Vector2& v, float value);

			// Operators
			Vector3 operator-() const;
			Vector3& operator=(const Vector3& v);
			bool operator==(const Vector3& v) const;
			bool operator!=(const Vector3& v) const;

			Vector3 operator+(float s) const;
			Vector3 operator+(const Vector3& v) const;
			Vector3& operator+=(float s);
			Vector3& operator+=(const Vector3& v);

			Vector3 operator-(float s) const;
			Vector3 operator-(const Vector3& v) const;
			Vector3& operator-=(float s);
			Vector3& operator-=(const Vector3& v);

			Vector3 operator*(float s) const;
			Vector3 operator*(const Vector3& v) const;
			Vector3& operator*=(float s);
			Vector3& operator*=(const Vector3& v);

			Vector3 operator/(float s) const;
			Vector3 operator/(const Vector3& v) const;
			Vector3& operator/=(float s);
			Vector3& operator/=(const Vector3& v);

			float GetLength() const;
			float GetLengthSquare() const;
			Vector3& Normalize();

			Vector3 GetNormalized() const;
			float Dot(const Vector3& v) const;
			Vector3 Cross(const Vector3& v) const;
			bool Equals(const Vector3& other, const float eps);
		};

	} 

}

#endif
