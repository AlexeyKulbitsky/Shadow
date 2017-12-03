#include "Vector3.h"

#include <cmath>

namespace sh
{

namespace math
{

	// Constructors
	Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vector3::Vector3(float s) : x(s), y(s), z(s) {}
	Vector3::Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
	Vector3::Vector3(const Vector2& v, float value) : x(v.x), y(v.y), z(value) {}

	// Operators
	Vector3 Vector3::operator-() const { return Vector3(-x, -y, -z); }
	Vector3& Vector3::operator=(const Vector3& v) { x = v.x; y = v.y; z = v.z; return *this; }
	bool Vector3::operator==(const Vector3& v) const { return v.x == x && v.y == y && v.z == z; }
	bool Vector3::operator!=(const Vector3& v) const { return v.x != x || v.y != y || v.z != z; }

	Vector3 Vector3::operator+(float s) const { return Vector3(x + s, y + s, z + s); }
	Vector3 Vector3::operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	Vector3& Vector3::operator+=(float s) { x += s; y += s; z += s; return *this; }
	Vector3& Vector3::operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }

	Vector3 Vector3::operator-(float s) const { return Vector3(x - s, y - s, z - s); }
	Vector3 Vector3::operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	Vector3& Vector3::operator-=(float s) { x -= s; y -= s; z -= s; return *this; }
	Vector3& Vector3::operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

	Vector3 Vector3::operator*(float s) const { return Vector3(x * s, y * s, z * s); }
	Vector3 Vector3::operator*(const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
	Vector3& Vector3::operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	Vector3& Vector3::operator*=(const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }

	Vector3 Vector3::operator/(float s) const { return Vector3(x / s, y / s, z / s); }
	Vector3 Vector3::operator/(const Vector3& v) const { return Vector3(x / v.x, y / v.y, z / v.z); }
	Vector3& Vector3::operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
	Vector3& Vector3::operator/=(const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

	float Vector3::GetLength() const
	{
		return Sqrt(x * x + y * y + z * z);
	}

	float Vector3::GetLengthSquare() const
	{
		return x * x + y * y + z * z;
	}

	Vector3& Vector3::Normalize()
	{
		float length = GetLength();
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}

	Vector3 Vector3::GetNormalized() const
	{
		float length = GetLength();
		return Vector3(x / length, y / length, z / length);
	}

	float Vector3::Dot(const Vector3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vector3 Vector3::Cross(const Vector3& v) const
	{
		return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	bool Vector3::Equals(const Vector3& other, const float eps)
	{
		return (std::abs(x - other.x) < eps) && (std::abs(y - other.y) < eps) && (std::abs(z - other.z) < eps);
	}

} // math

} // sh