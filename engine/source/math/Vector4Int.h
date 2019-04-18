#ifndef SHADOW_VECTOR4_INT_INCLUDE
#define SHADOW_VECTOR4_INT_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "math/Vector3.h"

namespace sh
{

	namespace math
	{
		class SHADOW_API Vector4Int
		{
		public:
			int32_t x, y, z, w;

			// Constructors
			Vector4Int();
			Vector4Int(int32_t _x, int32_t _y, int32_t _z, int32_t _w);
			Vector4Int(int32_t s);
			Vector4Int(const Vector4Int& v);

			// Operators
			Vector4Int operator-() const;
			Vector4Int& operator=(const Vector4Int& v);
			bool operator==(const Vector4Int& v) const;
			bool operator!=(const Vector4Int& v) const;

			Vector4Int operator+(int32_t s) const;
			Vector4Int operator+(const Vector4Int& v) const;
			Vector4Int& operator+=(int32_t s);
			Vector4Int& operator+=(const Vector4Int& v);

			Vector4Int operator-(int32_t s) const;
			Vector4Int operator-(const Vector4Int& v) const;
			Vector4Int& operator-=(int32_t s);
			Vector4Int& operator-=(const Vector4Int& v);

			Vector4Int operator*(int32_t s) const;
			Vector4Int operator*(const Vector4Int& v) const;
			Vector4Int& operator*=(int32_t s);
			Vector4Int& operator*=(const Vector4Int& v);

			Vector4Int operator/(int32_t s) const;
			Vector4Int operator/(const Vector4Int& v) const;
			Vector4Int& operator/=(int32_t s);
			Vector4Int& operator/=(const Vector4Int& v);
		};

	}

}

#endif