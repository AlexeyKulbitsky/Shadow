#include "Vector3Int.h"

namespace sh
{

namespace math
{

	// Constructors
	Vector3Int::Vector3Int() : x(0), y(0), z(0) {}
	Vector3Int::Vector3Int(int32_t _x, int32_t _y, int32_t _z) : x(_x), y(_y), z(_z) {}
	Vector3Int::Vector3Int(int32_t s) : x(s), y(s), z(s) {}
	Vector3Int::Vector3Int(const Vector3Int& v) : x(v.x), y(v.y), z(v.z) {}
	Vector3Int::Vector3Int(const Vector2Int& v, int32_t value) : x(v.x), y(v.y), z(value) {}

	// Operators
	Vector3Int Vector3Int::operator-() const { return Vector3Int(-x, -y, -z); }
	Vector3Int& Vector3Int::operator=(const Vector3Int& v) { x = v.x; y = v.y; z = v.z; return *this; }
	bool Vector3Int::operator==(const Vector3Int& v) const { return v.x == x && v.y == y && v.z == z; }
	bool Vector3Int::operator!=(const Vector3Int& v) const { return v.x != x || v.y != y || v.z != z; }

	Vector3Int Vector3Int::operator+(int32_t s) const { return Vector3Int(x + s, y + s, z + s); }
	Vector3Int Vector3Int::operator+(const Vector3Int& v) const { return Vector3Int(x + v.x, y + v.y, z + v.z); }
	Vector3Int& Vector3Int::operator+=(int32_t s) { x += s; y += s; z += s; return *this; }
	Vector3Int& Vector3Int::operator+=(const Vector3Int& v) { x += v.x; y += v.y; z += v.z; return *this; }

	Vector3Int Vector3Int::operator-(int32_t s) const { return Vector3Int(x - s, y - s, z - s); }
	Vector3Int Vector3Int::operator-(const Vector3Int& v) const { return Vector3Int(x - v.x, y - v.y, z - v.z); }
	Vector3Int& Vector3Int::operator-=(int32_t s) { x -= s; y -= s; z -= s; return *this; }
	Vector3Int& Vector3Int::operator-=(const Vector3Int& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

	Vector3Int Vector3Int::operator*(int32_t s) const { return Vector3Int(x * s, y * s, z * s); }
	Vector3Int Vector3Int::operator*(const Vector3Int& v) const { return Vector3Int(x * v.x, y * v.y, z * v.z); }
	Vector3Int& Vector3Int::operator*=(int32_t s) { x *= s; y *= s; z *= s; return *this; }
	Vector3Int& Vector3Int::operator*=(const Vector3Int& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }

	Vector3Int Vector3Int::operator/(int32_t s) const { return Vector3Int(x / s, y / s, z / s); }
	Vector3Int Vector3Int::operator/(const Vector3Int& v) const { return Vector3Int(x / v.x, y / v.y, z / v.z); }
	Vector3Int& Vector3Int::operator/=(int32_t s) { x /= s; y /= s; z /= s; return *this; }
	Vector3Int& Vector3Int::operator/=(const Vector3Int& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

} // math

} // sh