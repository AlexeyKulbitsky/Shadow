#ifndef SHADOW_RECT_INCLUDE
#define SHADOW_RECT_INCLUDE

#include "Vector2.h"

namespace sh
{

namespace math
{

	template<typename T>
	struct Rect
	{
		Rect() : upperLeftCorner(0, 0), lowerRightCorner(0, 0) { }
		Rect(T x1, T y1, T x2, T y2) : upperLeftCorner(x1, y1), lowerRightCorner(x2, y2) { }
		Rect(const Vector2<T> upperLeft, const Vector2<T> lowerRight) : upperLeftCorner(upperLeft), lowerRightCorner(lowerRight) { }

		bool operator==(const Rect<T>& other) const 
		{ 
			return upperLeftCorner == other.upperLeftCorner &&
				lowerRightCorner == other.lowerRightCorner;
		}

		bool operator!=(const Rect<T>& other) const 
		{
			return upperLeftCorner != other.upperLeftCorner ||
				lowerRightCorner != other.lowerRightCorner;
		}

		void Set(T x1, T y1, T x2, T y2)
		{
			upperLeftCorner.x = x1;
			upperLeftCorner.y = y1;
			lowerRightCorner.x = x2;
			lowerRightCorner.y = y2;
		}

		void Set(const Vector2<T> upperLeft, const Vector2<T> lowerRight)
		{
			upperLeftCorner = upperLeft;
			lowerRightCorner = lowerRight;
		}

		T GetWidth() const { return lowerRightCorner.x - upperLeftCorner.x; }
		T GetHeight() const { return lowerRightCorner.y - upperLeftCorner.y; }
		T GetArea() const { return GetWidth() * GetHeight(); }

		Vector2<T> GetSize() const { return Vector2<T>(GetWidth(), GetHeight()); }
		Vector2<T> GetCenter() const { return Vector2<T>((lowerRightCorner + upperLeftCorner) / 2); }
		Vector2<T> GetExtent() const { return lowerRightCorner - upperLeftCorner; }

		bool IsPointInside(const Vector2<T>& point) { 
			return upperLeftCorner.x <= point.x && upperLeftCorner.y <= point.y &&
			lowerRightCorner.x >= point.x && lowerRightCorner.y >= point.y;
		}

		bool IsPointInside(const T& x, const T& y)
		{
			return upperLeftCorner.x <= x && upperLeftCorner.y <= y &&
			lowerRightCorner.x >= x && lowerRightCorner.y >= y;
		}

		bool Intersects(const Rect<T>& other)
		{
			if (upperLeftCorner.x > other.lowerRightCorner.x ||
				other.upperLeftCorner.x > lowerRightCorner.x ||
				upperLeftCorner.y > other.lowerRightCorner.y ||
				other.upperLeftCorner.y > lowerRightCorner.y)
				return false;
			else
				return true;
		}

		Vector2<T> upperLeftCorner;
		Vector2<T> lowerRightCorner;
	};

} // math

} // sh

#endif