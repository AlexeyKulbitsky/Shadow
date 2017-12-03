#ifndef SHADOW_RECT_INCLUDE
#define SHADOW_RECT_INCLUDE

#include "Vector2Int.h"

#include "../CompileConfig.h"

namespace sh
{

namespace math
{

	class SHADOW_API Rect
	{
	public:
		Rect();
		Rect(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
		Rect(const Vector2Int upperLeft, const Vector2Int lowerRight);

		bool operator==(const Rect& other) const;
		bool operator!=(const Rect& other) const;

		void Set(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
		void Set(const Vector2Int& upperLeft, const Vector2Int& lowerRight);

		int32_t GetWidth() const;
		int32_t GetHeight() const;
		int32_t GetArea() const;

		Vector2Int GetSize() const;
		Vector2Int GetCenter() const;
		Vector2Int GetExtent() const;

		bool IsPointInside(const Vector2Int& point) const;
		bool IsPointInside(const int32_t& x, const int32_t& y) const;
		bool Intersects(const Rect& other) const;

		Vector2Int upperLeftCorner;
		Vector2Int lowerRightCorner;
	};

} // math

} // sh

#endif