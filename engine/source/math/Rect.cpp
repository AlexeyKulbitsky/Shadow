#include "math/Rect.h"

namespace sh
{

namespace math
{

	Rect::Rect() : upperLeftCorner(0, 0), lowerRightCorner(0, 0) { }
	Rect::Rect(int32_t x1, int32_t y1, int32_t x2, int32_t y2) : upperLeftCorner(x1, y1), lowerRightCorner(x2, y2) { }
	Rect::Rect(const Vector2Int upperLeft, const Vector2Int lowerRight) : upperLeftCorner(upperLeft), lowerRightCorner(lowerRight) { }

	bool Rect::operator==(const Rect& other) const
	{
		return upperLeftCorner == other.upperLeftCorner &&
			lowerRightCorner == other.lowerRightCorner;
	}

	bool Rect::operator!=(const Rect& other) const
	{
		return upperLeftCorner != other.upperLeftCorner ||
			lowerRightCorner != other.lowerRightCorner;
	}

	void Rect::Set(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
	{
		upperLeftCorner.x = x1;
		upperLeftCorner.y = y1;
		lowerRightCorner.x = x2;
		lowerRightCorner.y = y2;
	}

	void Rect::Set(const Vector2Int& upperLeft, const Vector2Int& lowerRight)
	{
		upperLeftCorner = upperLeft;
		lowerRightCorner = lowerRight;
	}

	int32_t Rect::GetWidth() const { return lowerRightCorner.x - upperLeftCorner.x; }
	int32_t Rect::GetHeight() const { return lowerRightCorner.y - upperLeftCorner.y; }
	int32_t Rect::GetArea() const { return GetWidth() * GetHeight(); }

	Vector2Int Rect::GetSize() const { return Vector2Int(GetWidth(), GetHeight()); }
	Vector2Int Rect::GetCenter() const { return Vector2Int((lowerRightCorner + upperLeftCorner) / 2); }
	Vector2Int Rect::GetExtent() const { return lowerRightCorner - upperLeftCorner; }

	bool Rect::IsPointInside(const Vector2Int& point) const
	{
		return upperLeftCorner.x <= point.x && upperLeftCorner.y <= point.y &&
			lowerRightCorner.x >= point.x && lowerRightCorner.y >= point.y;
	}

	bool Rect::IsPointInside(const int32_t& x, const int32_t& y) const
	{
		return upperLeftCorner.x <= x && upperLeftCorner.y <= y &&
			lowerRightCorner.x >= x && lowerRightCorner.y >= y;
	}

	bool Rect::Intersects(const Rect& other) const
	{
		return !(upperLeftCorner.x > other.lowerRightCorner.x ||
			other.upperLeftCorner.x > lowerRightCorner.x ||
			upperLeftCorner.y > other.lowerRightCorner.y ||
			other.upperLeftCorner.y > lowerRightCorner.y);
	}

} // math

} // sh