#pragma once
namespace sh
{

	namespace math
	{

		/*
		 * Rectangle template.
		 * Mostly used by 2D GUI elements and for 2D drawing methods.
		 * It has 2 positions instead of position and dimension and a fast
		 * method for collision detection with other rectangles and points.
		 *
		 * BEWARE:
		 * the origin of the onscreen GUI (Aurora, TouchMgr, ActionMgr) coordinate system is TOP LEFT.
		 * so y values are measured from top.
		 *
		 * O --> X
		 * |
		 * v
		 * Y
		 *
		 * don't get confused that m_upperLeftCorner.y is visually higher than m_lowerRightCorner.y,
		 * but the value is actually lower!!! this leads to a not working "IsPointInside" check.
		 * you can call Repair to fix that.
		 */
		template <typename T>
		struct rect
		{
			///////////////////////////////////////////////////////////////////////////////
			// constructors
			///////////////////////////////////////////////////////////////////////////////
			rect();
			rect(T x, T y, T x2, T y2);
			rect(const math::vec2<T>& upperLeft, const math::vec2<T>& lowerRight);

			///////////////////////////////////////////////////////////////////////////////
			// methods
			///////////////////////////////////////////////////////////////////////////////

			void set(T x, T y, T x2, T y2);
			void set(const math::vec2<T>& upperLeft, const math::vec2<T>& lowerRight);

			T getArea() const;
			T getWidth() const;
			T getHeight() const;
			math::vec2<T> getCenter() const;
			math::vec2<T> getSize() const;
			math::vec2<T> getExtent() const;

			void scale(T scale);

			//! If the lower right corner of the rect is smaller then the
			//! upper left, the points are swapped.
			void repair();

			//! Returns if the rect is valid to draw. It could be invalid
			//! if the UpperLeftCorner is lower or more right than the
			//! LowerRightCorner, or if any dimension is 0.
			bool isValid() const;

			///////////////////////////////////////////////////////////////////////////////
			// containment methods
			///////////////////////////////////////////////////////////////////////////////

				//! Returns if a 2d point is within this rectangle.
				//! \param pos: Position to test if it lies within this rectangle.
				//! \return Returns true if the position is within the rectangle, false if not.
			bool isPointInside(const math::vec2<T>& pos) const;

			//! Returns if the rectangle collides with another rectangle.
			bool isRectCollided(const rect<T>& other) const;

			//! Clips this rectangle with another one.
			void clipAgainst(const rect<T>& other);

			//! Moves this rectangle to fit inside another one.
			//! \return: returns true on success, false if not possible
			bool constrainTo(const rect<T>& other);

			//! Returns the closest point inside the rectangle to the given point
			math::vec2<T> getClosestPoint(const math::vec2<T>& pt) const;

			//grows with size in all directions (top, bottom, left, right)
			//the actual growth size size*2
			void grow(const T& size);
			void grow(const math::vec2<T>& size);

			void clampToMin(const T& size);
			void clampToMin(const math::vec2<T>& minSize);
			void clampToMax(const T& size);
			void clampToMax(const math::vec2<T>& maxSize);

			//! Adds a point to the rectangle, causing it to grow bigger,
			//! if point is outside of the box
			//! \param p: Point to add into the box.
			void addInternalPoint(const math::vec2<T>& p);

			//! Adds a point to the bounding rectangle, causing it to grow bigger,
			//! if point is outside of the box.
			//! \param x: X Coordinate of the point to add to this box.
			//! \param y: Y Coordinate of the point to add to this box.
			void addInternalPoint(T x, T y);

			///////////////////////////////////////////////////////////////////////////////
			// operators
			///////////////////////////////////////////////////////////////////////////////

			rect<T> operator+(const math::vec2<T>& pos) const;
			rect<T> operator-(const math::vec2<T>& pos) const;
			rect<T> operator*(const T& scale) const;
			rect<T> operator/(const T& scale) const;
			rect<T>& operator+=(const math::vec2<T>& pos);
			rect<T>& operator+=(const rect<T>& other);
			rect<T>& operator-=(const math::vec2<T>& pos);
			bool operator==(const rect<T>& other) const;
			bool operator!=(const rect<T>& other) const;
			bool operator<(const rect<T>& other) const;

			///////////////////////////////////////////////////////////////////////////////
			// data members
			///////////////////////////////////////////////////////////////////////////////
			math::vec2<T> upperLeftCorner;
			math::vec2<T> lowerRightCorner;
		};


	}

}
