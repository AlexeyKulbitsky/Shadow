#pragma once
namespace sh
{


	namespace math
	{

		//! Axis aligned bounding box in 3d dimensional space.
		template <typename T>
		struct aabb
		{
			///////////////////////////////////////////////////////////////////////////////
			// constructors
			///////////////////////////////////////////////////////////////////////////////
			aabb();
			//! Constructor with min edge and max edge.
			aabb(const vec3<T>& minPoint, const vec3<T>& maxPoint);
			//! Constructor with only one point.
			aabb(const vec3<T>& init);
			//! Constructor with min edge and max edge as single values, not vectors.
			aabb(T minx, T miny, T minz, T maxx, T maxy, T maxz);

			///////////////////////////////////////////////////////////////////////////////
			// methods
			///////////////////////////////////////////////////////////////////////////////

				//! Resets the bounding box to a one-point box.
				/** \param x X coord of the point.
				\param y Y coord of the point.
				\param z Z coord of the point. */
			void reset(T x, T y, T z);

			//! Resets the bounding box.
			/** \param initValue New box to set this one to. */
			void reset(const aabb<T>& initValue);

			//! Resets the bounding box.
			/** \param min Box min edge.
				\param max Box max edge.
			*/
			void reset(const vec3<T>& min, const vec3<T>& max);

			//! Resets the bounding box to a one-point box.
			/** \param initValue New point. */
			void reset(const vec3<T>& initValue);

			//! Resets the bounding box to an oriented bounding box
			void reset(const mat4<T>& mat, const aabb<T>& localBox);

			//! Get extent of the box
			/** \return Extent of the bounding box. */
			vec3<T> getExtent() const;

			//! Get the radius of the sphere containing the box
			/** \return Radius of the sphere containing the box. */
			T getRadius() const;

			//! Get the radius of the sphere containing the box
			/** \return Radius of the sphere containing the box. */
			T getRadiusFast() const;

			//! Get the squared radius of the sphere containing the box
			/** \return Squared radius of the sphere containing the box. */
			T getRadiusSq() const;

			//! Get the radius of the sphere containing the box
			/** \return Radius of the sphere containing the box. */
			T getInnerRadius() const;

			//! Get the squared radius of the sphere containing the box
			/** \return Squared radius of the sphere containing the box. */
			T getInnerRadiusSq() const;

			//! Stores all 8 edges/corners of the box into an array
			/** \param edges: an array of 8 edges. */
			void getEdges(vec3<T> edges[8]) const;

			//! Check if the box is empty.
			/** This means that there is no space between the min and max edge.
			\return True if box is empty, else false. */
			bool isEmpty() const;

			//! Repairs the box.
			/** Necessary if for example MinEdge and MaxEdge are swapped. */
			void repair();

			//! Calculates a new interpolated bounding box.
			/** \param other: other box to interpolate between
			\param d: value between 0.0f and 1.0f.
			\return Interpolated box. */
			aabb<T> getInterpolated(const aabb<T>& other, float d) const;

			//! Get center of the bounding box
			/** \return Center of the bounding box. */
			vec3<T> getCenter() const;

			//! Get the volume enclosed by the box in cubed units
			T getVolume() const;

			//! Get the surface area of the box in squared units
			T getArea() const;

			///////////////////////////////////////////////////////////////////////////////
			// containment
			///////////////////////////////////////////////////////////////////////////////

				//! Determines if a point is within this box.
				/** \param p: Point to check.
				\return True if the point is within the box and false if not */
			bool isPointInside(const vec3<T>& p) const;

			//! Determines if a point is within this box and its borders.
			/** \param p: Point to check.
			\return True if the point is within the box and false if not. */
			bool isPointTotalInside(const vec3<T>& p) const;

			//! Check if this box is completely inside the 'other' box.
			/** \param other: Other box to check against.
			\return True if this box is completly inside the other box,
			otherwise false. */
			bool isFullInside(const aabb<T>& other) const;

			//! Adds a point to the bounding box
			/** The box grows bigger, if point was outside of the box.
			\param p: Point to add into the box. */
			void addInternalPoint(const vec3<T>& p);

			//! Adds a point to the bounding box
			/** The box grows bigger, if point is outside of the box.
			\param x X coordinate of the point to add to this box.
			\param y Y coordinate of the point to add to this box.
			\param z Z coordinate of the point to add to this box. */
			void addInternalPoint(T x, T y, T z);

			//! Adds another bounding box
			/** The box grows bigger, if the new box was outside of the box.
			\param b: Other bounding box to add into this box. */
			void addInternalBox(const aabb<T>& b);

			// ! Adds an oriented box
			void addInternalBox(const mat4<T>& mat, const aabb<T>& b);

			///////////////////////////////////////////////////////////////////////////////
			// intersection
			///////////////////////////////////////////////////////////////////////////////

				//! Determines if the box intersects with another box.
				/** \param other: Other box to check a intersection with.
				\return True if there is an intersection with the other box,
				otherwise false. */
			bool intersectsWithBox(const aabb<T>& other) const;

			//! Tests if the box intersects with a line
			/** \param linemiddle Center of the line.
			\param linevect Vector of the line.
			\param halflength Half length of the line.
			\return True if there is an intersection, else false. */
			bool intersectsWithLine(const vec3<T>& linemiddle, const vec3<T>& linevect, T halflength) const;

			bool intersectsWithLine(const line3<T>& line) const;

			/** Check the intersection of a 3d ray against the box3.
			@param ray The ray to test for collision in the box local coordinate system.
			@param outIntersectionNear closest intersection point
			@param outIntersectionFar furthest intersection point
			@return true if there is intersection and the closest and furthest intersection are written if so; false is returned otherwise.
			*/
			bool intersectsWithRay(const line3<T>& ray, vec3<T>& outIntersectionNear, vec3<T>& outIntersectionFar) const;

			bool intersectsWithRay(const line3<T>& ray, float& outIntersectionNearDistance, float& outIntersectionFarDistance) const;

			///////////////////////////////////////////////////////////////////////////////
			// operators
			///////////////////////////////////////////////////////////////////////////////

			bool operator==(const aabb<T>& other) const;
			bool operator!=(const aabb<T>& other) const;

			///////////////////////////////////////////////////////////////////////////////
			// members
			///////////////////////////////////////////////////////////////////////////////

			vec3<T> minPoint;
			vec3<T> maxPoint;
		};

	}

}
