#pragma once
namespace sh
{


	namespace math
	{

		//! 2D line between two points with intersection methods.
		template <typename T>
		struct line2
		{
			///////////////////////////////////////////////////////////////////////////////
			// constructors
			///////////////////////////////////////////////////////////////////////////////
				/** line from (0,0) to (1,1) */
			line2();
			//! Constructor with two points
			line2(T xa, T ya, T xb, T yb);
			//! Constructor with two points as vectors
			line2(const math::vec2<T>& start, const math::vec2<T>& end);

			///////////////////////////////////////////////////////////////////////////////
			// methods
			///////////////////////////////////////////////////////////////////////////////

				//! Set this line to a new line going through the two points.
			void setLine(const T& xa, const T& ya, const T& xb, const T& yb);
			//! Set this line to a new line going through the two points.
			void setLine(const math::vec2<T>& nstart, const math::vec2<T>& nend);
			inline void setLine(const line2<T>& line);

			//! Get length of line
			/** \return Length of line. */
			T getLength() const;

			//! Get squared length of line
			/** \return Squared length of line. */
			T getLengthSQ() const;

			//! Get middle of line
			/** \return Center of line. */
			math::vec2<T> getMiddle() const;

			//! Get vector of line
			/** \return vector of line. */
			math::vec2<T> getVector() const;

			//! Get vector of line
			/** \return vector of line. */
			math::vec2<T> getVectorNormalized() const;

			///////////////////////////////////////////////////////////////////////////////
			// intersection
			///////////////////////////////////////////////////////////////////////////////

				//! Check if the given point is between start and end of the line.
				/** Assumes that the point is already somewhere on the line.
				\param point The point to test.
				\return True if point is on the line between start and end, else false.
				*/
			bool isPointBetweenStartAndEnd(const math::vec2<T>& point) const;

			//! Get the closest point on this line to a point
			/** \param point The point to compare to.
			\return The nearest point which is part of the line. */
			math::vec2<T> getClosestPoint(const math::vec2<T>& point) const;

			//! Check if two segments intersect. This method returns the intersection point if it exists.
			/**
			\param other: The other Line to check against
			\param point: If it is not null it will be set to the intersection point, in case there was intersection.
			\param outMu: If it is not null it is set to the mu intersection value (from 0 to 1), in case there was intersection.
			\return True if there is an intersection.
			*/
			bool getIntersectionWithLine(const line2<T>& other, math::vec2<T>* point = 0, T* outMu = 0);

			//! Check if the line intersects with a circle
			/**
			\param sorigin: Origin of the circle.
			\param sradius: Radius of the circle.
			\param outdistance: The distance to the first intersection point.
			\return True if there is an intersection.
			If there is one, the distance to the first intersection point
			is stored in outdistance.
			*/
			bool getIntersectionWithCircle(math::vec2<T> sorigin, T sradius, double* outdistance = 0) const;

			//! Check the intersection of this line against a Rect.
			/**
			\param rect: The rect to check against
			\param point: If it is not null it is set to the intersection point, in case there was intersection.
			\param outMu: If it is not null it is set to the mu intersection value (from 0 to 1), in case there was intersection.
			\return True if there is an intersection.
			*/
			bool getIntersectionWithRect(const rect<T>& rect, math::vec2<T>* point = 0, T* outMu = 0);

			///////////////////////////////////////////////////////////////////////////////
			// operators
			///////////////////////////////////////////////////////////////////////////////

			line2<T> operator+(const math::vec2<T>& point) const;
			line2<T> operator-(const math::vec2<T>& point) const;
			line2<T>& operator+=(const math::vec2<T>& point);
			line2<T>& operator-=(const math::vec2<T>& point);
			bool operator==(const line2<T>& other) const;
			bool operator!=(const line2<T>& other) const;

			///////////////////////////////////////////////////////////////////////////////
			// members
			///////////////////////////////////////////////////////////////////////////////

			vec2<T> start;
			vec2<T> end;
		};

		typedef line2<float>	line2f;
		typedef line2<double>	line2d;

	}//namespace math

}
