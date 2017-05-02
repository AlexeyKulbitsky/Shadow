#ifndef SHADOW_PLANE_INCLUDE
#define SHADOW_PLANE_INCLUDE

#include "Vector3.h"
#include "../Types.h"

namespace sh
{
	namespace math
	{
		template<typename T>
		struct Plane
		{
			Vector3<T> normal;
			T d;

			// Constructors
			Plane(): normal(0,1,0) { RecalculateD(Vector3<T>(0,0,0)); }

			////////////////////////////////////////////////////////
		
			Plane(const Vector3<T>& MPoint, const Vector3<T>& Normal) 
				: normal(Normal) { RecalculateD(MPoint); }
		
			////////////////////////////////////////////////////////

			Plane(T px, T py, T pz, T nx, T ny, T nz) 
				: normal(nx, ny, nz) { RecalculateD(Vector3<T>(px, py, pz)); }
		
			////////////////////////////////////////////////////////

			Plane(const Vector3<T>& point1, const Vector3<T>& point2, const Vector3<T>& point3)
			{ 
				SetPlane(point1, point2, point3); 
			}

			////////////////////////////////////////////////////////
	
			Plane(const Vector3<T> & Normal, const T D) : normal(Normal), d(D) { }

			////////////////////////////////////////////////////////

			// operators
			//inline bool operator==(const Plane<T>& other) const { return (equals(D, other.D) && Normal==other.Normal);}

			Plane<T>& operator=(const Plane<T>& p) { normal = p.normal; d = p.d; return *this; }
			////////////////////////////////////////////////////////

			//inline bool operator!=(const Plane<T>& other) const { return !(*this == other);}

			////////////////////////////////////////////////////////

			// functions

			void SetPlane(const Vector3<T>& point, const Vector3<T>& nvector)
			{
				normal = nvector;
				RecalculateD(point);
			}

			////////////////////////////////////////////////////////

			void SetPlane(const Vector3<T>& nvect, T D)
			{
				normal = nvect;
				d = D;
			}

			////////////////////////////////////////////////////////

			void SetPlane(const Vector3<T>& point1, const Vector3<T>& point2, const Vector3<T>& point3)
			{
				// creates the plane from 3 memberpoints
				normal = (point2 - point1).Cross(point3 - point1);
				normal.Normalize();

				RecalculateD(point1);
			}

			////////////////////////////////////////////////////////

			//! Recalculates the distance from origin by applying a new member point to the plane.
			void RecalculateD(const Vector3<T>& MPoint)
			{
				d = - MPoint.Dot(normal);
			}

			////////////////////////////////////////////////////////

			bool GetIntersectionWithLine(const Vector3<T>& linePoint,
				const Vector3<T>& lineVect,
				Vector3<T>& outIntersection) const
			{
				T t2 = normal.Dot(lineVect);

				if (t2 == 0)
					return false;

				T t =- (normal.Dot(linePoint) + d) / t2;
				outIntersection = linePoint + (lineVect * t);
				return true;
			}

			////////////////////////////////////////////////////////

			f32 GetKnownIntersectionWithLine(const Vector3<T>& linePoint1,
			const Vector3<T>& linePoint2) const
			{
				Vector3<T> vect = linePoint2 - linePoint1;
				T t2 = (f32)normal.Dot(vect);
				return (f32)-((normal.Dot(linePoint1) + d) / t2);
			}

			////////////////////////////////////////////////////////

			T GetDistanceTo(const Vector3<T>& point) const
			{
				return point.Dot(normal) + d;
			}

			////////////////////////////////////////////////////////
	
		};
	}
}

#endif