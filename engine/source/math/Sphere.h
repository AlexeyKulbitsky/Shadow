#ifndef SHADOW_SPHERE_INCLUDE
#define SHADOW_SPHERE_INCLUDE

#include "Vector3.h"

namespace sh
{
	namespace math
	{
		template<typename T>
		struct Sphere
		{
			T GetRadius() const { return radius; }
			void SetRadius(T r) { radius = r; }

			const Vector3<T>& GetCentre() const { return centre; }
			void SetCentre(const Vector3<T> c) { centre = c; }

			Vector3<T> centre;
			T radius;
		};
	}
}

#endif