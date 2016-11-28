#ifndef SHADOW_QUATERNION_INCLUDE
#define SHADOW_QUATERNION_INCLUDE

#include "Vector3.h"

namespace sh
{
	namespace math
	{
		template<typename T>
		struct Quaternion
		{
			T x, y, z, w;

			Quaternion() : x(0), y(0), z(0), w(0) {}
			Quaternion(T _x, T _y, T _z, T _w) : x(_x), y(_x), z(_x), w(_w) {}
			explicit Quaternion(T s) : x(s), y(s), z(s), w(s) {}
			Quaternion(const Quaternion<T>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
			Quaternion(const Vector3<T>& axis, T angle)
			{
				T halfAngle = angle / 2;
				T cosA = Cos(halfAngle);
				T sinA = Sin(halfAngle);
				x = axis.x * sinA;
				y = axis.y * sinA;
				z = axis.z * sinA;
				w = cosA;
			}

		};
	}
}

#endif
