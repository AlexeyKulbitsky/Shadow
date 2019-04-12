#include "math/Vector4Int.h"

namespace sh
{

namespace math
{

	// Constructors
	Vector4Int::Vector4Int() : x(0), y(0), z(0), w(0) {}
	Vector4Int::Vector4Int(int32_t _x, int32_t _y, int32_t _z, int32_t _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector4Int::Vector4Int(int32_t s) : x(s), y(s), z(s), w(s) {}
	Vector4Int::Vector4Int(const Vector4Int& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	// Operators
	Vector4Int Vector4Int::operator-() const { return Vector4Int(-x, -y, -z, -w); }
	Vector4Int& Vector4Int::operator=(const Vector4Int& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
	bool Vector4Int::operator==(const Vector4Int& v) const { return v.x == x && v.y == y && v.z == z && v.w == w; }
	bool Vector4Int::operator!=(const Vector4Int& v) const { return v.x != x || v.y != y || v.z != z || v.w != w; }

	Vector4Int Vector4Int::operator+(int32_t s) const { return Vector4Int(x + s, y + s, z + s, w + s); }
	Vector4Int Vector4Int::operator+(const Vector4Int& v) const { return Vector4Int(x + v.x, y + v.y, z + v.z, w + v.w); }
	Vector4Int& Vector4Int::operator+=(int32_t s) { x += s; y += s; z += s; w += s; return *this; }
	Vector4Int& Vector4Int::operator+=(const Vector4Int& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }

	Vector4Int Vector4Int::operator-(int32_t s) const { return Vector4Int(x - s, y - s, z - s, w - s); }
	Vector4Int Vector4Int::operator-(const Vector4Int& v) const { return Vector4Int(x - v.x, y - v.y, z - v.z, w - v.w); }
	Vector4Int& Vector4Int::operator-=(int32_t s) { x -= s; y -= s; z -= s; w -= s; return *this; }
	Vector4Int& Vector4Int::operator-=(const Vector4Int& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }

	Vector4Int Vector4Int::operator*(int32_t s) const { return Vector4Int(x * s, y * s, z * s, w * s); }
	Vector4Int Vector4Int::operator*(const Vector4Int& v) const { return Vector4Int(x * v.x, y * v.y, z * v.z, w * v.w); }
	Vector4Int& Vector4Int::operator*=(int32_t s) { x *= s; y *= s; z *= s; w *= s; return *this; }
	Vector4Int& Vector4Int::operator*=(const Vector4Int& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }

	Vector4Int Vector4Int::operator/(int32_t s) const { return Vector4Int(x / s, y / s, z / s, w / s); }
	Vector4Int Vector4Int::operator/(const Vector4Int& v) const { return Vector4Int(x / v.x, y / v.y, z / v.z, w / v.w); }
	Vector4Int& Vector4Int::operator/=(int32_t s) { x /= s; y /= s; z /= s; w /= s; return *this; }
	Vector4Int& Vector4Int::operator/=(const Vector4Int& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

} // math

} // sh