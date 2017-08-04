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
		AABB()
			: minPoint(-0.5f)
			, maxPoint(0.5f)
		{
		}

		AABB(const Vector3<T>& min, const Vector3<T>& max)
			: minPoint(min)
			, maxPoint(max)
		{
		}

		AABB(const Vector3<T>& initialPoint)
			: minPoint(initialPoint)
			, maxPoint(initialPoint)
		{
		}

		AABB<T>& operator=(const AABB<T>& b) { minPoint = b.minPoint; maxPoint = b.maxPoint; return *this; }

		Vector3<T> GetExtent() const 
		{
			return maxPoint - minPoint;
		}

		Vector3<T> GetCenter() const 
		{
			return (minPoint + maxPoint) * 0.5f;
		}

		T GetRadius() const 
		{
			return GetExtent().GetLength() * 0.5f;
		}

		void GetEdges(Vector3<T> edges[8]) const 
		{
			/*
			Edges are stored in this way:
			  /3--------/7
			 /  |      / |
			/   |     /  |
			1---------5  |
			|   2- - -| -6
			|  /      |  /
			|/        | /
			0---------4/
			*/

			const Vector3<T> middlePoint = GetCenter();
			const Vector3<T> diagonal = middlePoint - maxPoint;

			edges[0] = vec3<T>(middlePoint.x + diagonal.x, middlePoint.y + diagonal.y, middlePoint.z + diagonal.z);
			edges[1] = vec3<T>(middlePoint.x + diagonal.x, middlePoint.y - diagonal.y, middlePoint.z + diagonal.z);
			edges[2] = vec3<T>(middlePoint.x + diagonal.x, middlePoint.y + diagonal.y, middlePoint.z - diagonal.z);
			edges[3] = vec3<T>(middlePoint.x + diagonal.x, middlePoint.y - diagonal.y, middlePoint.z - diagonal.z);
			edges[4] = vec3<T>(middlePoint.x - diagonal.x, middlePoint.y + diagonal.y, middlePoint.z + diagonal.z);
			edges[5] = vec3<T>(middlePoint.x - diagonal.x, middlePoint.y - diagonal.y, middlePoint.z + diagonal.z);
			edges[6] = vec3<T>(middlePoint.x - diagonal.x, middlePoint.y + diagonal.y, middlePoint.z - diagonal.z);
			edges[7] = vec3<T>(middlePoint.x - diagonal.x, middlePoint.y - diagonal.y, middlePoint.z - diagonal.z);
		}

		bool IsPointInside(const Vector3<T>& point)
		{
			return (
				point.x >= minPoint.x && point.x <= maxPoint.x &&
				point.y >= minPoint.y && point.y <= maxPoint.y &&
				point.z >= minPoint.z && point.z <= maxPoint.z
				);
		}

		void AddPoint(T x, T y, T z)
		{
			if (x > maxPoint.x) maxPoint.x = x;
			if (y > maxPoint.y) maxPoint.y = y;
			if (z > maxPoint.z) maxPoint.z = z;

			if (x < minPoint.x) minPoint.x = x;
			if (y < minPoint.y) minPoint.y = y;
			if (z < minPoint.z) minPoint.z = z;
		}

		void AddPoint(const Vector3<T>& point)
		{
			AddPoint(point.x, point.y, point.z);
		}

		void AddBox(const AABB<T>& box)
		{
			AddPoint(box.minPoint);
			AddPoint(box.maxPoint);
		}

		Vector3<T> minPoint;
		Vector3<T> maxPoint;
	};

} //

} // sh

#endif