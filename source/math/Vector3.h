#ifndef SHADOW_VECTOR3_INCLUDE
#define SHADOW_VECTOR3_INCLUDE

#include "Vector2.h"

namespace sh
{

	namespace math
	{
		template<typename T>
		struct Vector3
		{
			T x, y, z;

			// Constructors
			Vector3() : x(0), y(0), z(0) {}
			Vector3(T _x, T _y, T _z) : x(_x), y(_x), z(_x) {}
			explicit Vector3(T s) : x(s), y(s), z(s) {}
			Vector3(const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {}
			Vector3(const Vector2<T>& v, T value) : x(v.x), y(v.y), z(value) {}

			// Operators
			Vector3<T> operator-() const { return Vector3<T>(-z, -y, -z); }
			Vector3<T>& operator=(const Vector3<T>& v) { x = v.x; y = v.y; z = v.z; return *this; }

			Vector3<T> operator+(T s) const { return Vector3<T>(x + s, y + s, z + s); }
			Vector3<T> operator+(const Vector3<T>& v) const { return Vector3<T>(x + v.x, y + v.y, z + v.z); }
			Vector3<T>& operator+=(T s) { x += s; y += s; z += s; return *this; }
			Vector3<T>& operator+=(const Vector3<T>& v) { x += v.x; y += v.y; z += v.z; return *this; }

			Vector3<T> operator-(T s) const { return Vector3<T>(x - s, y - s, z - s); }
			Vector3<T> operator-(const Vector3<T>& v) const { return Vector3<T>(x - v.x, y - v.y, z - v.z); }
			Vector3<T>& operator-=(T s) { x -= s; y -= s; z -= s; return *this; }
			Vector3<T>& operator-=(const Vector3<T>& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

			Vector3<T> operator*(T s) const { return Vector3<T>(x * s, y * s, z * s); }
			Vector3<T> operator*(const Vector3<T>& v) const { return Vector3<T>(x * v.x, y * v.y, z * v.z); }
			Vector3<T>& operator*=(T s) { x *= s; y *= s; z *= s; return *this; }
			Vector3<T>& operator*=(const Vector3<T>& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }

			Vector3<T> operator/(T s) const { return Vector3<T>(x / s, y / s, z / s); }
			Vector3<T> operator/(const Vector3<T>& v) const { return Vector3<T>(x / v.x, y / v.y, z / v.z); }
			Vector3<T>& operator/=(T s) { x /= s; y /= s; z /= s; return *this; }
			Vector3<T>& operator/=(const Vector3<T>& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

			T GetLength() const
			{
				return Sqrt(x * x + y * y + z * z);
			}

			Vector3<T>& Normalize()
			{
				T length = GetLength();
				x /= length;
				y /= length;
				z /= length;
				return *this;
			}

			Vector3<T> GetNormalized() const
			{
				T length = GetLength();
				return Vector3<T>(x / length, y / length, z / length);
			}
		};

	} 

}

#endif
