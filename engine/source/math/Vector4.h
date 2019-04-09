#ifndef SHADOW_VECTOR4_INCLUDE
#define SHADOW_VECTOR4_INCLUDE

#include "common/Prerequisities.h"
#include "Vector3.h"

namespace sh
{

	namespace math
	{
		class SHADOW_API Vector4
		{
		public:
			float x, y, z, w;

			// Constructors
			Vector4();
			Vector4(float _x, float _y, float _z, float _w);
			Vector4(float s);
			Vector4(const Vector4& v);
			Vector4(const Vector3& v, float value);

			// Operators
			Vector4 operator-() const;
			Vector4& operator=(const Vector4& v);
			Vector4& operator=(const Vector3& v);
			bool operator==(const Vector4& v) const;
			bool operator!=(const Vector4& v) const;

			Vector4 operator+(float s) const;
			Vector4 operator+(const Vector4& v) const;
			Vector4& operator+=(float s);
			Vector4& operator+=(const Vector4& v);

			Vector4 operator-(float s) const;
			Vector4 operator-(const Vector4& v) const;
			Vector4& operator-=(float s);
			Vector4& operator-=(const Vector4& v);

			Vector4 operator*(float s) const;
			Vector4 operator*(const Vector4& v) const;
			Vector4& operator*=(float s);
			Vector4& operator*=(const Vector4& v);

			Vector4 operator/(float s) const;
			Vector4 operator/(const Vector4& v) const;
			Vector4& operator/=(float s);
			Vector4& operator/=(const Vector4& v);

			float GetLength() const;
			Vector4& Normalize();
			Vector4 GetNormalized() const;
		};

	}

}

#endif