#pragma once
namespace sh
{

	namespace math
	{

		template <typename T> struct vec3;
		template <typename T> struct plane;
		template <typename T> struct line3;
		template <typename T> struct aabb;

		//! 3d triangle template class
		template <typename T>
		struct triangle3
		{
			triangle3();
			triangle3(vec3<T> v1, vec3<T> v2, vec3<T> v3);

			bool operator==(const triangle3<T>& other) const;

			bool operator!=(const triangle3<T>& other) const;

			//! Determines if the triangle is totally inside a bounding box.
			/** \param box Box to check.
			\return True if triangle is within the box, otherwise false. */
			bool isTotalInsideBox(const aabb<T>& box) const;

			//! Get the closest point on a triangle to a point on the same plane.
			/** \param p Point which must be on the same plane as the triangle.
			\return The closest point of the triangle */
			vec3<T> closestPointOnTriangle(const vec3<T>& p) const;

			//! Check if a point is inside the triangle
			/** \param p Point to test. Assumes that this point is already
			on the plane of the triangle.
			\return True if the point is inside the triangle, otherwise false. */
			bool isPointInside(const vec3<T>& p) const;

			//! Check if a point is inside the triangle.
			/** This method is an implementation of the example used in a
			paper by Kasper Fauerby original written by Keidy from
			Mr-Gamemaker.
			\param p Point to test. Assumes that this point is already
			on the plane of the triangle.
			\return True if point is inside the triangle, otherwise false. */
			bool isPointInsideFast(const vec3<T>& p) const;


			//! Get an intersection with a 3d line.
			/** \param line Line to intersect with.
			\param outIntersection Place to store the intersection point, if there is one.
			\return True if there was an intersection, false if not. */
			bool getIntersectionWithLimitedLine(const line3<T>& line,
				vec3<T>& outIntersection) const;


			//! Get an intersection with a 3d line.
			/** Please note that also points are returned as intersection which
			are on the line, but not between the start and end point of the line.
			If you want the returned point be between start and end
			use getIntersectionWithLimitedLine().
			\param linePoint Point of the line to intersect with.
			\param lineVect Vector of the line to intersect with.
			\param outIntersection Place to store the intersection point, if there is one.
			\return True if there was an intersection, false if there was not. */
			bool getIntersectionWithLine(const vec3<T>& linePoint,
				const vec3<T>& lineVect, vec3<T>& outIntersection) const;

			bool getIntersectionWithLineFast(const vec3<T>& linePoint,
				const vec3<T>& lineVect, vec3<T>& outIntersection) const;

			// triangle ray barycentric test
			bool intersectRay(const vec3<T>& rayOrigin, const vec3<T>& rayDir, bool cullbackface, T& t, T& u, T& v) const;

			//! Calculates the intersection between a 3d line and the plane the triangle is on.
			/** \param lineVect Vector of the line to intersect with.
			\param linePoint Point of the line to intersect with.
			\param outIntersection Place to store the intersection point, if there is one.
			\return True if there was an intersection, else false. */
			bool getIntersectionOfPlaneWithLine(const vec3<T>& linePoint,
				const vec3<T>& lineVect, vec3<T>& outIntersection) const;

			bool getIntersectionOfPlaneWithLineFast(const vec3<T>& linePoint,
				const vec3<T>& lineVect, vec3<T>& outIntersection) const;


			//! Get the normal of the triangle.
			/** Please note: The normal is not always normalized. */
			vec3<T> getNormal() const;

			//! Test if the triangle would be front or backfacing from any point.
			/** Thus, this method assumes a camera position from which the
			triangle is definitely visible when looking at the given direction.
			Do not use this method with points as it will give wrong results!
			\param lookDirection Look direction.
			\return True if the plane is front facing and false if it is backfacing. */
			bool isFrontFacing(const vec3<T>& lookDirection) const;

			//! Get the plane of this triangle.
			plane<T> getPlane() const;

			//! Get the area of the triangle
			T getArea() const;

			//! Get the squared area of the triangle
			T getAreaSq() const;

			void set(const vec3<T>& a, const vec3<T>& b, const vec3<T>& c);

			vec3<T>  pointA;
			vec3<T>  pointB;
			vec3<T>  pointC;

		private:
			bool isOnSameSide(const vec3<T>& p1, const vec3<T>& p2,
				const vec3<T>& a, const vec3<T>& b) const;
		};

	}


}
