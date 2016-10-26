#pragma once
namespace sh
{


	namespace math
	{

		//! 3D line between two points with intersection methods.
		template <typename T>
		struct line3
		{
			///////////////////////////////////////////////////////////////////////////////
			// constructors
			///////////////////////////////////////////////////////////////////////////////
				/** line from (0,0,0) to (1,1,1) */
			line3();
			//! Constructor with two points
			line3(T xa, T ya, T za, T xb, T yb, T zb);
			//! Constructor with two points as vectors
			line3(const vec3<T>& start, const vec3<T>& end);

			///////////////////////////////////////////////////////////////////////////////
			// methods
			///////////////////////////////////////////////////////////////////////////////

				//! Set this line to a new line going through the two points.
			void setLine(const T& xa, const T& ya, const T& za, const T& xb, const T& yb, const T& zb);
			//! Set this line to a new line going through the two points.
			void setLine(const vec3<T>& nstart, const vec3<T>& nend);
			//! Set this line to new line given as parameter.
			void setLine(const line3<T>& line);

			//! Get length of line
			/** \return Length of line. */
			T getLength() const;

			//! Get squared length of line
			/** \return Squared length of line. */
			T getLengthSQ() const;

			//! Get middle of line
			/** \return Center of line. */
			vec3<T> getMiddle() const;

			//! Get vector of line
			/** \return vector of line. */
			vec3<T> getVector() const;

			//! Get vector of line
			/** \return vector of line. */
			vec3<T> getVectorNormalized() const;

			///////////////////////////////////////////////////////////////////////////////
			// intersection
			///////////////////////////////////////////////////////////////////////////////

			bool isPointOnLine(const vec3<T>& point) const;

			bool isBetweenPoints(const vec3<T>& point, const vec3<T>& begin, const vec3<T>& end) const;

			//! Check if the given point is between start and end of the line.
			/** Assumes that the point is already somewhere on the line.
			\param point The point to test.
			\return True if point is on the line between start and end, else false.
			*/
			bool isPointBetweenStartAndEnd(const vec3<T>& point) const;

			//! Get the closest point on this line to a point
			/** \param point The point to compare to.
			\return The nearest point which is part of the line. */
			vec3<T> getClosestPoint(const vec3<T>& point) const;

			//! Project the given point on the line and get a normalized position within the longitude of the line.
			/** \param point The point to project.
			\return The normalized position (mu) within the line. 0.0f means on m_start, 1.0f means on m_end */
			T getClosestPointProjected(const vec3<T>& point) const;

			bool isIntersectingWithSphere(const vec3<T>& sphereCenter, T sphereRadius) const;

			//! Check if the line intersects with a sphere
			/** \param sphereCenter: Center of the sphere.
			\param sphereRadius: Radius of the sphere.
			\param outdistance: The distance to the first intersection point.
			\return True if there is an intersection. If there is one, the distance to the first intersection point is stored in outdistance. */
			bool getIntersectionWithSphere(const vec3<T>& sphereCenter, T sphereRadius, double& outdistance) const;

			//! Check if the line intersects with a sphere
			/** \param sphereCenter: Center of the sphere.
			\param sphereRadius: Radius of the sphere.
			\param outdistance1: The distance to the first intersection point. A negative distance means the collision is before m_start (that is opposite to GetVector() direction).
			\param outdistance2: The distance to the second intersection point. A negative distance means the collision is before m_start (that is opposite to GetVector() direction).
			\return True if there is an intersection. If there is one, the distance to the first intersection point is stored in outdistance. */
			bool getIntersectionWithSphere(const vec3<T>& sphereCenter, T sphereRadius, double& outdistance1, double& outdistance2) const;

			bool getIntersectionWithLine(const line3<T>& line, vec3<T>& intersection) const;

			///////////////////////////////////////////////////////////////////////////////
			// operators
			///////////////////////////////////////////////////////////////////////////////

			line3<T> operator+(const vec3<T>& point) const;
			line3<T> operator-(const vec3<T>& point) const;
			line3<T>& operator+=(const vec3<T>& point);
			line3<T>& operator-=(const vec3<T>& point);
			bool operator==(const line3<T>& other) const;
			bool operator!=(const line3<T>& other) const;

			///////////////////////////////////////////////////////////////////////////////
			// members
			///////////////////////////////////////////////////////////////////////////////

			vec3<T> start;
			vec3<T> end;
		};

	}//namespace math

}
