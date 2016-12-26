#ifndef SHADOW_VECTOR4_INCLUDE
#define SHADOW_VECTOR4_INCLUDE

#include "Vector3.h"

namespace sh
{

	namespace math
	{
		template<typename T>
		struct Vector4
		{
			T x, y, z, w;

			// Constructors
			Vector4() : x(0), y(0), z(0), w(0) {}
			Vector4(T _x, T _y, T _z, T _w) : x(_x), y(_x), z(_x), w(_w) {}
			explicit Vector4(T s) : x(s), y(s), z(s), w(s) {}
			Vector4(const Vector4<T>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
			Vector4(const Vector3<T>& v, T value) : x(v.x), y(v.y), z(v.z), w(value) {}

			// Operators
			Vector4<T> operator-() const { return Vector4<T>(-z, -y, -z, -w); }
			Vector4<T>& operator=(const Vector4<T>& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

			Vector4<T> operator+(T s) const { return Vector4<T>(x + s, y + s, z + s, w + s); }
			Vector4<T> operator+(const Vector4<T>& v) const { return Vector4<T>(x + v.x, y + v.y, z + v.z, w + v.w); }
			Vector4<T>& operator+=(T s) { x += s; y += s; z += s; w += s; return *this; }
			Vector4<T>& operator+=(const Vector4<T>& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }

			Vector4<T> operator-(T s) const { return Vector4<T>(x - s, y - s, z - s, w - s); }
			Vector4<T> operator-(const Vector4<T>& v) const { return Vector4<T>(x - v.x, y - v.y, z - v.z, w - v.w); }
			Vector4<T>& operator-=(T s) { x -= s; y -= s; z -= s; w -= s; return *this; }
			Vector4<T>& operator-=(const Vector4<T>& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }

			Vector3<T> operator*(T s) const { return Vector4<T>(x * s, y * s, z * s, w * s); }
			Vector3<T> operator*(const Vector4<T>& v) const { return Vector4<T>(x * v.x, y * v.y, z * v.z, w * v.w); }
			Vector3<T>& operator*=(T s) { x *= s; y *= s; z *= s; w *= s; return *this; }
			Vector3<T>& operator*=(const Vector4<T>& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }

			Vector4<T> operator/(T s) const { return Vector4<T>(x / s, y / s, z / s, w / s); }
			Vector4<T> operator/(const Vector4<T>& v) const { return Vector4<T>(x / v.x, y / v.y, z / v.z, w / v.w); }
			Vector4<T>& operator/=(T s) { x /= s; y /= s; z /= s; w /= s; return *this; }
			Vector4<T>& operator/=(const Vector4<T>& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

			T GetLength() const
			{
				return Sqrt(x * x + y * y + z * z + w * w);
			}

			Vector4<T>& Normalize()
			{
				T length = GetLength();
				x /= length;
				y /= length;
				z /= length;
				w /= length;
				return *this;
			}

			Vector4<T> GetNormalized() const
			{
				T length = GetLength();
				return Vector4<T>(x / length, y / length, z / length, w / length);
			}
		};

	}

}

#endif