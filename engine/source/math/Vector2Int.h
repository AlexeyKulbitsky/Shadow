#ifndef SHADOW_VECTOR2_INT_INCLUDE
#define SHADOW_VECTOR2_INT_INCLUDE

#include "common/Prerequisities.h"
#include "FunctionsCommon.h"

namespace sh
{

	namespace math
	{
		class SHADOW_API Vector2Int
		{
		public:
			int32_t x, y;

			// Constructors
			Vector2Int();
			Vector2Int(int32_t _x, int32_t _y);
			Vector2Int(int32_t s);
			Vector2Int(const Vector2Int& v);

			// Operators
			Vector2Int operator-() const;
			Vector2Int& operator=(const Vector2Int& v);
			bool operator==(const Vector2Int& v) const;
			bool operator!=(const Vector2Int& v) const;

			Vector2Int operator+(int32_t s) const;
			Vector2Int operator+(const Vector2Int& v) const;
			Vector2Int& operator+=(int32_t s);
			Vector2Int& operator+=(const Vector2Int& v);

			Vector2Int operator-(int32_t s) const;
			Vector2Int operator-(const Vector2Int& v) const;
			Vector2Int& operator-=(int32_t s);
			Vector2Int& operator-=(const Vector2Int& v);

			Vector2Int operator*(int32_t s) const;
			Vector2Int operator*(const Vector2Int& v) const;
			Vector2Int& operator*=(int32_t s);
			Vector2Int& operator*=(const Vector2Int& v);

			Vector2Int operator/(int32_t s) const;
			Vector2Int operator/(const Vector2Int& v) const;
			Vector2Int& operator/=(int32_t s);
			Vector2Int& operator/=(const Vector2Int& v);
		};

	}

}

#endif
