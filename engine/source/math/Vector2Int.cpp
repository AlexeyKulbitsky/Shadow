#include "math/Vector2Int.h"

namespace sh
{

namespace math
{

	// Constructors
	Vector2Int::Vector2Int() : x(0), y(0) {}
	Vector2Int::Vector2Int(int32_t _x, int32_t _y) : x(_x), y(_y) {}
	Vector2Int::Vector2Int(int32_t s) : x(s), y(s) {}
	Vector2Int::Vector2Int(const Vector2Int& v) : x(v.x), y(v.y) {}

	// Operators
	Vector2Int Vector2Int::operator-() const { return Vector2Int(-x, -y); }
	Vector2Int& Vector2Int::operator=(const Vector2Int& v) { x = v.x; y = v.y; return *this; }
	bool Vector2Int::operator==(const Vector2Int& v) const { return v.x == x && v.y == y; }
	bool Vector2Int::operator!=(const Vector2Int& v) const { return v.x != x || v.y != y; }

	Vector2Int Vector2Int::operator+(int32_t s) const { return Vector2Int(x + s, y + s); }
	Vector2Int Vector2Int::operator+(const Vector2Int& v) const { return Vector2Int(x + v.x, y + v.y); }
	Vector2Int& Vector2Int::operator+=(int32_t s) { x += s; y += s; return *this; }
	Vector2Int& Vector2Int::operator+=(const Vector2Int& v) { x += v.x; y += v.y; return *this; }

	Vector2Int Vector2Int::operator-(int32_t s) const { return Vector2Int(x - s, y - s); }
	Vector2Int Vector2Int::operator-(const Vector2Int& v) const { return Vector2Int(x - v.x, y - v.y); }
	Vector2Int& Vector2Int::operator-=(int32_t s) { x -= s; y -= s; return *this; }
	Vector2Int& Vector2Int::operator-=(const Vector2Int& v) { x -= v.x; y -= v.y; return *this; }

	Vector2Int Vector2Int::operator*(int32_t s) const { return Vector2Int(x * s, y * s); }
	Vector2Int Vector2Int::operator*(const Vector2Int& v) const { return Vector2Int(x * v.x, y * v.y); }
	Vector2Int& Vector2Int::operator*=(int32_t s) { x *= s; y *= s; return *this; }
	Vector2Int& Vector2Int::operator*=(const Vector2Int& v) { x *= v.x; y *= v.y; return *this; }

	Vector2Int Vector2Int::operator/(int32_t s) const { return Vector2Int(x / s, y / s); }
	Vector2Int Vector2Int::operator/(const Vector2Int& v) const { return Vector2Int(x / v.x, y / v.y); }
	Vector2Int& Vector2Int::operator/=(int32_t s) { x /= s; y /= s; return *this; }
	Vector2Int& Vector2Int::operator/=(const Vector2Int& v) { x /= v.x; y /= v.y; return *this; }

} // math

} // sh