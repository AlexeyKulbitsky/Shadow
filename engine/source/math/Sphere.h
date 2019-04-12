#ifndef SHADOW_SPHERE_INCLUDE
#define SHADOW_SPHERE_INCLUDE

#include "common/Prerequisities.h"
#include "math/Vector3.h"

namespace sh
{
	namespace math
	{
		class SHADOW_API Sphere
		{
		public:
			float GetRadius() const;
			void SetRadius(float r);

			const Vector3& GetCentre() const;
			void SetCentre(const Vector3& c);

			Vector3 centre;
			float radius;
		};
	}
}

#endif