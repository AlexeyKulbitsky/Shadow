#pragma once
namespace sh
{

	namespace math
	{

		//! Enumeration for intersection relations of 3d objects
		enum RelationEnum
		{
			k_relFront = 0,
			k_relBack,
			k_relPlanar,
			k_relSpanning,
			k_relClipping
		};

		//! Template plane class with some intersection testing methods.
		template <class T>
		struct plane
		{
			///////////////////////////////////////////////////////////////////////////////
			// constructors
			///////////////////////////////////////////////////////////////////////////////
			plane();
			plane(const math::vec3<T>& point, const math::vec3<T>& normal);
			plane(T px, T py, T pz, T nx, T ny, T nz);
			plane(const math::vec3<T>& point1, const math::vec3<T>& point2, const math::vec3<T>& point3);

			///////////////////////////////////////////////////////////////////////////////
			// operators
			///////////////////////////////////////////////////////////////////////////////
			bool operator==(const plane<T>& other) const;
			bool operator!=(const plane<T>& other) const;

			///////////////////////////////////////////////////////////////////////////////
			// methods
			///////////////////////////////////////////////////////////////////////////////
			void setPlane(const math::vec3<T>& point, const math::vec3<T>& normal);
			void setPlane(const math::vec3<T>& normal, T d);
			void setPlane(const math::vec3<T>& point1, const math::vec3<T>& point2, const math::vec3<T>& point3);

			//! Recalculates the distance from origin by applying a new member point to the plane.
			void recalculateD(const math::vec3<T>& MPoint);
			//! Gets a member point of the plane.
			math::vec3<T> getMemberPoint() const;

			//! Test if the triangle would be front or backfacing from any point.
			/** Thus, this method assumes a camera position from
			which the triangle is definitely visible when looking into
			the given direction.
			Note that this only works if the normal is Normalized.
			Do not use this method with points as it will give wrong results!
			\param lookDirection: Look direction.
			\return True if the plane is front facing and
			false if it is backfacing. */
			bool isFrontFacing(const math::vec3<T>& lookDirection) const;

			//! Get the distance to a point.
			/** Note that this only works if the normal is normalized. */
			T getDistanceTo(const math::vec3<T>& point) const;

			//! Project a point on the plane.
			math::vec3<T> projectPoint(const math::vec3<T>& point) const;

			///! Computes the inverse halfspace
			plane<T>& invert();

			///////////////////////////////////////////////////////////////////////////////
			// intersection methods
			///////////////////////////////////////////////////////////////////////////////
				//! Get an intersection with a 3d line.
				/** \param lineVect Vector of the line to intersect with.
				\param linePoint Point of the line to intersect with.
				\param outIntersection Place to store the intersection point, if there is one.
				\return True if there was an intersection, false if there was not.
				*/
			bool getIntersectionWithLine(const math::vec3<T>& linePoint, const math::vec3<T>& lineVect, math::vec3<T>& outIntersection) const;

			//! Get percentage of line between two points where an intersection with this plane happens.
			/** Only useful if known that there is an intersection.
			\param linePoint1 Point1 of the line to intersect with.
			\param linePoint2 Point2 of the line to intersect with.
			\return Where on a line between two points an intersection with this plane happened.
			For example, 0.5 is returned if the intersection happened exactly in the middle of the two points.
			*/
			float getKnownIntersectionWithLine(const math::vec3<T>& linePoint1, const math::vec3<T>& linePoint2) const;

			//! Get an intersection with a 3d line, limited between two 3d points.
			/** \param linePoint1 Point 1 of the line.
			\param linePoint2 Point 2 of the line.
			\param outIntersection Place to store the intersection point, if there is one.
			\return True if there was an intersection, false if there was not.
			*/
			bool getIntersectionWithLimitedLine(const math::vec3<T>& linePoint1, const math::vec3<T>& linePoint2, math::vec3<T>& outIntersection) const;

			//! Classifies the relation of a point to this plane.
			/** \param point Point to classify its relation.
			\return ISREL3D_FRONT if the point is in front of the plane,
			ISREL3D_BACK if the point is behind of the plane, and
			ISREL3D_PLANAR if the point is within the plane. */
			RelationEnum classifyPointRelation(const math::vec3<T>& point) const;

			//! Tests if there is an intersection with the other plane
			/** \return True if there is a intersection. */
			bool existsIntersection(const plane<T>& other) const;

			//! Intersects this plane with another.
			/** \param other Other plane to intersect with.
			\param outLinePoint Base point of intersection line.
			\param outLineVect Vector of intersection.
			\return True if there is a intersection, false if not. */
			bool getIntersectionWithPlane(const plane<T>& other, math::vec3<T>& outLinePoint, math::vec3<T>& outLineVect) const;

			//! Get the intersection point with two other planes if there is one.
			bool getIntersectionWithPlanes(const plane<T>& o1, const plane<T>& o2, math::vec3<T>& outPoint) const;

			//! Translate the plane according to offset
			/** \param offset Translation offset. */
			void translate(const math::vec3<T>& offset);

			///////////////////////////////////////////////////////////////////////////////
			// overlap methods
			///////////////////////////////////////////////////////////////////////////////
				///! Checks for plane AABB overlap
				// Return values:
				// 0: box is totally outside of the plane
				// 1: box is totally inside of the plane
				// 2: box is partially inside of the plane
			int32_t overlap(const math::vec3<T>& boxCenter, math::vec3<T>& boxHalfsize) const;

			//! Normal of the plane.
			math::vec3<T>  normal;
			//! Distance from origin.
			T             d;
		};

		typedef plane<float>  planef;
		typedef plane<double>  planed;

	}

}
