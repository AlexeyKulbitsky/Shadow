#pragma once

///////////////////////////////////////////////////////////////////////////////
// Defines CircleIntersectPrimitive functions
///////////////////////////////////////////////////////////////////////////////
namespace sh
{


	namespace math
	{
		template<class T>
		uint8_t circleIntersectCircle(
			const math::vec2<T>& origin, const T r1, const math::vec2<T>& dir, const T r2, math::vec2<T>& t, math::vec2<T>& t1);

	}//namespace math

}
