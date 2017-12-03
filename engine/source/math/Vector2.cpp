#include "Vector2.h"

namespace sh
{

namespace math
{

	// Constructors
	Vector2::Vector2() : x(0.0f), y(0.0f) {}
	Vector2::Vector2(float _x, float _y) : x(_x), y(_y) {}
	Vector2::Vector2(float s) : x(s), y(s) {}
	Vector2::Vector2(const Vector2& v) : x(v.x), y(v.y) {}

	// Operators
	Vector2 Vector2::operator-() const { return Vector2(-x, -y); }
	Vector2& Vector2::operator=(const Vector2& v) { x = v.x; y = v.y; return *this; }
	bool Vector2::operator==(const Vector2& v) const { return v.x == x && v.y == y; }
	bool Vector2::operator!=(const Vector2& v) const { return v.x != x || v.y != y; }

	Vector2 Vector2::operator+(float s) const { return Vector2(x + s, y + s); }
	Vector2 Vector2::operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	Vector2& Vector2::operator+=(float s) { x += s; y += s; return *this; }
	Vector2& Vector2::operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }

	Vector2 Vector2::operator-(float s) const { return Vector2(x - s, y - s); }
	Vector2 Vector2::operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	Vector2& Vector2::operator-=(float s) { x -= s; y -= s; return *this; }
	Vector2& Vector2::operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }

	Vector2 Vector2::operator*(float s) const { return Vector2(x * s, y * s); }
	Vector2 Vector2::operator*(const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
	Vector2& Vector2::operator*=(float s) { x *= s; y *= s; return *this; }
	Vector2& Vector2::operator*=(const Vector2& v) { x *= v.x; y *= v.y; return *this; }

	Vector2 Vector2::operator/(float s) const { return Vector2(x / s, y / s); }
	Vector2 Vector2::operator/(const Vector2& v) const { return Vector2(x / v.x, y / v.y); }
	Vector2& Vector2::operator/=(float s) { x /= s; y /= s; return *this; }
	Vector2& Vector2::operator/=(const Vector2& v) { x /= v.x; y /= v.y; return *this; }

	float Vector2::GetLength() const
	{
		return Sqrt(x * x + y * y);
	}

	Vector2& Vector2::Normalize()
	{
		float length = GetLength();
		x /= length;
		y /= length;
		return *this;
	}

	Vector2 Vector2::GetNormalized() const
	{
		float length = GetLength();
		return Vector2(x / length, y / length);
	}

} // math

} // sh