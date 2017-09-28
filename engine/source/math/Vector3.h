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
			Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
			explicit Vector3(T s) : x(s), y(s), z(s) {}
			Vector3(const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {}
			Vector3(const Vector2<T>& v, T value) : x(v.x), y(v.y), z(value) {}

			template<typename U>
			Vector3(const Vector3<U>& v) 
				: x(static_cast<T>(v.x))
				, y(static_cast<T>(v.y))
				, z(static_cast<T>(v.z))
			{ }

			template<typename U>
			Vector3(const Vector2<U>& v, T value) 
				: x(static_cast<T>(v.x))
				, y(static_cast<T>(v.y))
				, z(value)
			{ }

			// Operators
			Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }
			Vector3<T>& operator=(const Vector3<T>& v) { x = v.x; y = v.y; z = v.z; return *this; }
			bool operator==(const Vector3<T>& v) const { return v.x == x && v.y == y && v.z == z; }
			bool operator!=(const Vector3<T>& v) const { return v.x != x || v.y != y || v.z != z; }

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

			T GetLengthSquare() const
			{
				return x * x + y * y + z * z;
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

			T Dot(const Vector3<T>& v) const
			{
				return x * v.x + y * v.y + z * v.z;
			}

			Vector3<T> Cross(const Vector3<T>& v) const
			{
				return Vector3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
			}

			bool Equals(const Vector3<T>& other, const T& eps)
			{
				return (std::abs(x - other.x) < eps) && (std::abs(y - other.y) < eps) && (std::abs(z - other.z) < eps);
			}
		};

	} 

}

#endif
