#ifndef SHADOW_AABB_INCLUDE
#define SHADOW_AABB_INCLUDE

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

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

			edges[0] = Vector3<T>(middlePoint.x + diagonal.x, middlePoint.y + diagonal.y, middlePoint.z + diagonal.z);
			edges[1] = Vector3<T>(middlePoint.x + diagonal.x, middlePoint.y - diagonal.y, middlePoint.z + diagonal.z);
			edges[2] = Vector3<T>(middlePoint.x + diagonal.x, middlePoint.y + diagonal.y, middlePoint.z - diagonal.z);
			edges[3] = Vector3<T>(middlePoint.x + diagonal.x, middlePoint.y - diagonal.y, middlePoint.z - diagonal.z);
			edges[4] = Vector3<T>(middlePoint.x - diagonal.x, middlePoint.y + diagonal.y, middlePoint.z + diagonal.z);
			edges[5] = Vector3<T>(middlePoint.x - diagonal.x, middlePoint.y - diagonal.y, middlePoint.z + diagonal.z);
			edges[6] = Vector3<T>(middlePoint.x - diagonal.x, middlePoint.y + diagonal.y, middlePoint.z - diagonal.z);
			edges[7] = Vector3<T>(middlePoint.x - diagonal.x, middlePoint.y - diagonal.y, middlePoint.z - diagonal.z);
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

		void Reset(const Vector3<T>& min, const Vector3<T>& max)
		{
			minPoint = min;
			maxPoint = max;
		}

		void Transform(const Matrix4<T>& mat)
		{
			Vector3<T> edges[8];
			GetEdges(edges);

			auto v0 = mat * Vector4<T>(edges[0], 1);
			minPoint = Vector3<T>(v0.x, v0.y, v0.z);
			maxPoint = Vector3<T>(v0.x, v0.y, v0.z);

			for (u32 i = 1U; i < 8; ++i)
			{
				auto v = mat * Vector4<T>(edges[i], 1);
				AddPoint(v.x, v.y, v.z);
			}
		}

		AABB<T> GetTransformed(const Matrix4<T>& mat) const
		{
			AABB<T> result(minPoint, maxPoint);
			result.Transform(mat);
			return result;
		}

		Vector3<T> minPoint;
		Vector3<T> maxPoint;
	};

} //

} // sh

#endif