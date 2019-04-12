#ifndef SHADOW_AABB_INCLUDE
#define SHADOW_AABB_INCLUDE

#include "common/Prerequisities.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4.h"

namespace sh
{

namespace math
{

	class SHADOW_API AABB
	{
	public:
		AABB();
		AABB(const Vector3& min, const Vector3& max);
		AABB(const Vector3& initialPoint);

		AABB& operator=(const AABB& b);

		Vector3 GetExtent() const;
		Vector3 GetCenter() const;
		float GetRadius() const;
		void GetEdges(Vector3 edges[8]) const;
		bool IsPointInside(const Vector3& point);
		void AddPoint(float x, float y, float z);
		void AddPoint(const Vector3& point);
		void AddBox(const AABB& box);
		void Reset(const Vector3& min, const Vector3& max);
		void Transform(const Matrix4& mat);
		AABB GetTransformed(const Matrix4& mat) const;

		Vector3 minPoint;
		Vector3 maxPoint;
	};

} //

} // sh

#endif