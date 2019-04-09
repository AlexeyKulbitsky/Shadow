#ifndef SHADOW_PLANE_INCLUDE
#define SHADOW_PLANE_INCLUDE

#include "common/Prerequisities.h"
#include "Vector3.h"

namespace sh
{
	namespace math
	{
		class SHADOW_API Plane
		{
		public:
			Vector3 normal;
			float d;

			// Constructors
			Plane();
			Plane(const Vector3& MPoint, const Vector3& Normal);
			Plane(float px, float py, float pz, float nx, float ny, float nz);
			Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3);
			Plane(const Vector3& Normal, const float D);

			Plane& operator=(const Plane& p);

			void SetPlane(const Vector3& point, const Vector3& nvector);
			void SetPlane(const Vector3& nvect, float D);
			void SetPlane(const Vector3& point1, const Vector3& point2, const Vector3& point3);

			void RecalculateD(const Vector3& MPoint);
			bool GetIntersectionWithLine(const Vector3& linePoint,
				const Vector3& lineVect,
				Vector3& outIntersection) const;
			float GetKnownIntersectionWithLine(const Vector3& linePoint1, const Vector3& linePoint2) const;
			float GetDistanceTo(const Vector3& point) const;
	
		};
	}
}

#endif