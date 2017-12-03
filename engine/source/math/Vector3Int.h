#ifndef SHADOW_VECTOR3_INT_INCLUDE
#define SHADOW_VECTOR3_INT_INCLUDE

#include "Vector2Int.h"

#include "../CompileConfig.h"

namespace sh
{

	namespace math
	{
		class SHADOW_API Vector3Int
		{
		public:
			int32_t x, y, z;

			// Constructors
			Vector3Int();
			Vector3Int(int32_t _x, int32_t _y, int32_t _z);
			Vector3Int(int32_t s);
			Vector3Int(const Vector3Int& v);
			Vector3Int(const Vector2Int& v, int32_t value);

			// Operators
			Vector3Int operator-() const;
			Vector3Int& operator=(const Vector3Int& v);
			bool operator==(const Vector3Int& v) const;
			bool operator!=(const Vector3Int& v) const;

			Vector3Int operator+(int32_t s) const;
			Vector3Int operator+(const Vector3Int& v) const;
			Vector3Int& operator+=(int32_t s);
			Vector3Int& operator+=(const Vector3Int& v);

			Vector3Int operator-(int32_t s) const;
			Vector3Int operator-(const Vector3Int& v) const;
			Vector3Int& operator-=(int32_t s);
			Vector3Int& operator-=(const Vector3Int& v);

			Vector3Int operator*(int32_t s) const;
			Vector3Int operator*(const Vector3Int& v) const;
			Vector3Int& operator*=(int32_t s);
			Vector3Int& operator*=(const Vector3Int& v);

			Vector3Int operator/(int32_t s) const;
			Vector3Int operator/(const Vector3Int& v) const;
			Vector3Int& operator/=(int32_t s);
			Vector3Int& operator/=(const Vector3Int& v);
		};

	}

}

#endif
