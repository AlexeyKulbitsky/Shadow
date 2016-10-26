#pragma once

///////////////////////////////////////////////////////////////////////////////
// Defines RayIntersectPrimitive functions
///////////////////////////////////////////////////////////////////////////////
namespace sh
{

	namespace math
	{
		template<class T>
		bool rayIntersectPlane(
			const math::vec3<T>& origin, const math::vec3<T>& dir,
			const math::plane<T>& plane,
			T& t);

		template<class T>
		int rayIntersectAABB(
			const math::vec3<T>& rayOrigin, const math::vec3<T>& rayDir,
			const math::vec3<T>& boxCenter, const math::vec3<T>& boxHalfsize,
			T& tmin, T& tmax);

		template<class T>
		int rayIntersectSphere(
			const math::vec3<T>& rayOrigin, const math::vec3<T>& rayDirection,
			const math::vec3<T>& sphereCenter, T radius,
			T& t0, T& t1);

		template<class T>
		int rayIntersectHemisphere(
			const math::vec3<T>& rayOrigin, const math::vec3<T>& rayDirection,
			const math::vec3<T>& sphereCenter, const math::vec3<T>& sphereNormal, T radius,
			T& t0, T& t1);

		template<class T>
		bool rayIntersectDisk(const math::vec3<T>& rayOrigin, const math::vec3<T>& rayDir,
			const math::vec3<T>& circleCenter, const math::vec3<T>& circleNormal, T circleRadius,
			T& t);

		// Ray cylinder intersection test
		// Returns number of intersection points
		// rayOrigin,rayDir: ray origin and direction (Ray must be specified in cylinder object coordinates)
		// Cylinder center is (0,0,0) and cylinder axis is 0,0,1
		// length,radius: cylinder length and radius
		// t: reference 2-float array indicating distance to intersection points
		// intersectDisks: enables intersection with cone disks
		template<class T>
		int rayIntersectCylinder(
			const math::vec3<T>& p, const math::vec3<T>& d,
			T length, T radius,
			T* t, bool intersectDisks);

		// Capsule center is (0,0,0) and capsule axis is 0,0,1
		template<class T>
		int rayIntersectCapsule(
			const math::vec3<T>& rayOrigin, const math::vec3<T>& rayDir,
			T height, T radius,
			T& t0, T& t1);


	}//namespace math

}
