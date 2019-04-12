#ifndef SHADOW_VECTOR2_INCLUDE
#define SHADOW_VECTOR2_INCLUDE

#include "common/Prerequisities.h"
#include "math/FunctionsCommon.h"

namespace sh
{

	namespace math
	{
		class SHADOW_API Vector2
		{
		public:
			float x, y;

			// Constructors
			Vector2();
			Vector2(float _x, float _y);
			Vector2(float s);
			Vector2(const Vector2& v);

			// Operators
			Vector2 operator-() const;
			Vector2& operator=(const Vector2& v);
			bool operator==(const Vector2& v) const;
			bool operator!=(const Vector2& v) const;

			Vector2 operator+(float s) const;
			Vector2 operator+(const Vector2& v) const;
			Vector2& operator+=(float s);
			Vector2& operator+=(const Vector2& v);

			Vector2 operator-(float s) const;
			Vector2 operator-(const Vector2& v) const;
			Vector2& operator-=(float s);
			Vector2& operator-=(const Vector2& v);

			Vector2 operator*(float s) const;
			Vector2 operator*(const Vector2& v) const;
			Vector2& operator*=(float s);
			Vector2& operator*=(const Vector2& v);

			Vector2 operator/(float s) const;
			Vector2 operator/(const Vector2& v) const;
			Vector2& operator/=(float s);
			Vector2& operator/=(const Vector2& v);

			float GetLength() const;
			Vector2& Normalize();
			Vector2 GetNormalized() const;
		};

	}

}

#endif