#ifndef SHADOW_FUNCTIONS_RAY_INTERSECT_INCLUDE
#define SHADOW_FUNCTIONS_RAY_INTERSECT_INCLUDE

#include "Vector3.h"

namespace sh
{
	namespace math
	{
		template<class T>
		inline int RayIntersectSphere(const Vector3<T>& rayOrigin, const Vector3<T>& rayDirection, const Vector3<T>& sphereCenter, T radius, T& t0, T& t1)
		{
			// christer ericson code
			Vector3<T> m = rayOrigin - sphereCenter;
			T b = m.Dot(rayDirection);
			T c = m.Dot(m) - radius * radius;

			// exit if ray origin outside sphere ( c > 0) and r pointing away from sphere (b > 0)
			// if (c > 0.0f && b > 0.0f) return 0;

			T discr = b*b - c;
			if (discr < (T)0)
			{
				return 0;
			}

			T sqdiscr = Sqrt(discr);
			t0 = -b - sqdiscr;
			t1 = -b + sqdiscr;
			return 2;
		}
	}
}

#endif