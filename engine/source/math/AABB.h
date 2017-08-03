#ifndef SHADOW_AABB_INCLUDE
#define SHADOW_AABB_INCLUDE

#include "Vector3.h"

namespace sh
{

namespace math
{

	template<typename T>
	struct AABB
	{
		Vector3<T> minPoint;
		Vector3<T> maxPoint;
	};

} //

} // sh

#endif