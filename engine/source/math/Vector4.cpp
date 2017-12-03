#include "Vector4.h"

namespace sh
{

namespace math
{

	// Constructors
	Vector4::Vector4() : x(0), y(0), z(0), w(0) {}
	Vector4::Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector4::Vector4(float s) : x(s), y(s), z(s), w(s) {}
	Vector4::Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	Vector4::Vector4(const Vector3& v, float value) : x(v.x), y(v.y), z(v.z), w(value) {}

	// Operators
	Vector4 Vector4::operator-() const { return Vector4(-x, -y, -z, -w); }
	Vector4& Vector4::operator=(const Vector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
	Vector4& Vector4::operator=(const Vector3& v) { x = v.x; y = v.y; z = v.z; return *this; }
	bool Vector4::operator==(const Vector4& v) const { return v.x == x && v.y == y && v.z == z && v.w == w; }
	bool Vector4::operator!=(const Vector4& v) const { return v.x != x || v.y != y || v.z != z || v.w != w; }

	Vector4 Vector4::operator+(float s) const { return Vector4(x + s, y + s, z + s, w + s); }
	Vector4 Vector4::operator+(const Vector4& v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
	Vector4& Vector4::operator+=(float s) { x += s; y += s; z += s; w += s; return *this; }
	Vector4& Vector4::operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }

	Vector4 Vector4::operator-(float s) const { return Vector4(x - s, y - s, z - s, w - s); }
	Vector4 Vector4::operator-(const Vector4& v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
	Vector4& Vector4::operator-=(float s) { x -= s; y -= s; z -= s; w -= s; return *this; }
	Vector4& Vector4::operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }

	Vector4 Vector4::operator*(float s) const { return Vector4(x * s, y * s, z * s, w * s); }
	Vector4 Vector4::operator*(const Vector4& v) const { return Vector4(x * v.x, y * v.y, z * v.z, w * v.w); }
	Vector4& Vector4::operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
	Vector4& Vector4::operator*=(const Vector4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }

	Vector4 Vector4::operator/(float s) const { return Vector4(x / s, y / s, z / s, w / s); }
	Vector4 Vector4::operator/(const Vector4& v) const { return Vector4(x / v.x, y / v.y, z / v.z, w / v.w); }
	Vector4& Vector4::operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }
	Vector4& Vector4::operator/=(const Vector4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

	float Vector4::GetLength() const
	{
		return Sqrt(x * x + y * y + z * z + w * w);
	}

	Vector4& Vector4::Normalize()
	{
		float length = GetLength();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
		return *this;
	}

	Vector4 Vector4::GetNormalized() const
	{
		float length = GetLength();
		return Vector4(x / length, y / length, z / length, w / length);
	}

} // math

} // sh