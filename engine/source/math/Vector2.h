#ifndef SHADOW_VECTOR2_INCLUDE
#define SHADOW_VECTOR2_INCLUDE

#include "FunctionsCommon.h"

namespace sh
{

	namespace math
	{
		template<typename T>
		struct Vector2
		{
			T x, y;

			// Constructors
			Vector2() : x(0), y(0) {}
			Vector2(T _x, T _y) : x(_x), y(_y) {}
			explicit Vector2(T s) : x(s), y(s) {}
			Vector2(const Vector2<T>& v) : x(v.x), y(v.y) {}

			// Operators
			Vector2<T> operator-() const { return Vector2<T>(-x, -y); }
			Vector2<T>& operator=(const Vector2<T>& v) { x = v.x; y = v.y; return *this; }
			bool operator==(const Vector2<T>& v) const { return v.x == x && v.y == y; }

			Vector2<T> operator+(T s) const { return Vector2<T>(x + s, y + s); }
			Vector2<T> operator+(const Vector2<T>& v) const { return Vector2<T>(x + v.x, y + v.y); }
			Vector2<T>& operator+=(T s) { x += s; y += s; return *this; }
			Vector2<T>& operator+=(const Vector2<T>& v) { x += v.x; y += v.y; return *this; }

			Vector2<T> operator-(T s) const { return Vector2<T>(x - s, y - s); }
			Vector2<T> operator-(const Vector2<T>& v) const { return Vector2<T>(x - v.x, y - v.y); }
			Vector2<T>& operator-=(T s) { x -= s; y -= s; return *this; }
			Vector2<T>& operator-=(const Vector2<T>& v) { x -= v.x; y -= v.y; return *this; }

			Vector2<T> operator*(T s) const { return Vector2<T>(x * s, y * s); }
			Vector2<T> operator*(const Vector2<T>& v) const { return Vector2<T>(x * v.x, y * v.y); }
			Vector2<T>& operator*=(T s) { x *= s; y *= s; return *this; }
			Vector2<T>& operator*=(const Vector2<T>& v) { x *= v.x; y *= v.y; return *this; }

			Vector2<T> operator/(T s) const { return Vector2<T>(x / s, y / s); }
			Vector2<T> operator/(const Vector2<T>& v) const { return Vector2<T>(x / v.x, y / v.y); }
			Vector2<T>& operator/=(T s) { x /= s; y /= s; return *this; }
			Vector2<T>& operator/=(const Vector2<T>& v) { x /= v.x; y /= v.y; return *this; }

			T GetLength() const 
			{ 
				return Sqrt(x * x + y * y); 
			}

			Vector2<T>& Normalize()
			{ 
				T length = GetLength(); 
				x /= length; 
				y /= length; 
				return *this;
			}

			Vector2<T> GetNormalized() const
			{
				T length = GetLength();
				return Vector2<T>(x / length, y / length);
			}
		};

	}

}

#endif