#include "AABB.h"

namespace sh
{

namespace math
{

	AABB::AABB()
		: minPoint(-0.5f)
		, maxPoint(0.5f)
	{
	}

	AABB::AABB(const Vector3& min, const Vector3& max)
		: minPoint(min)
		, maxPoint(max)
	{
	}

	AABB::AABB(const Vector3& initialPoint)
		: minPoint(initialPoint)
		, maxPoint(initialPoint)
	{
	}

	AABB& AABB::operator=(const AABB& b) { minPoint = b.minPoint; maxPoint = b.maxPoint; return *this; }

	Vector3 AABB::GetExtent() const
	{
		return maxPoint - minPoint;
	}

	Vector3 AABB::GetCenter() const
	{
		return (minPoint + maxPoint) * 0.5f;
	}

	float AABB::GetRadius() const
	{
		return GetExtent().GetLength() * 0.5f;
	}

	void AABB::GetEdges(Vector3 edges[8]) const
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

		const Vector3 middlePoint = GetCenter();
		const Vector3 diagonal = middlePoint - maxPoint;

		edges[0] = Vector3(middlePoint.x + diagonal.x, middlePoint.y + diagonal.y, middlePoint.z + diagonal.z);
		edges[1] = Vector3(middlePoint.x + diagonal.x, middlePoint.y - diagonal.y, middlePoint.z + diagonal.z);
		edges[2] = Vector3(middlePoint.x + diagonal.x, middlePoint.y + diagonal.y, middlePoint.z - diagonal.z);
		edges[3] = Vector3(middlePoint.x + diagonal.x, middlePoint.y - diagonal.y, middlePoint.z - diagonal.z);
		edges[4] = Vector3(middlePoint.x - diagonal.x, middlePoint.y + diagonal.y, middlePoint.z + diagonal.z);
		edges[5] = Vector3(middlePoint.x - diagonal.x, middlePoint.y - diagonal.y, middlePoint.z + diagonal.z);
		edges[6] = Vector3(middlePoint.x - diagonal.x, middlePoint.y + diagonal.y, middlePoint.z - diagonal.z);
		edges[7] = Vector3(middlePoint.x - diagonal.x, middlePoint.y - diagonal.y, middlePoint.z - diagonal.z);
	}

	bool AABB::IsPointInside(const Vector3& point)
	{
		return (
			point.x >= minPoint.x && point.x <= maxPoint.x &&
			point.y >= minPoint.y && point.y <= maxPoint.y &&
			point.z >= minPoint.z && point.z <= maxPoint.z
			);
	}

	void AABB::AddPoint(float x, float y, float z)
	{
		if (x > maxPoint.x) maxPoint.x = x;
		if (y > maxPoint.y) maxPoint.y = y;
		if (z > maxPoint.z) maxPoint.z = z;

		if (x < minPoint.x) minPoint.x = x;
		if (y < minPoint.y) minPoint.y = y;
		if (z < minPoint.z) minPoint.z = z;
	}

	void AABB::AddPoint(const Vector3& point)
	{
		AddPoint(point.x, point.y, point.z);
	}

	void AABB::AddBox(const AABB& box)
	{
		AddPoint(box.minPoint);
		AddPoint(box.maxPoint);
	}

	void AABB::Reset(const Vector3& min, const Vector3& max)
	{
		minPoint = min;
		maxPoint = max;
	}

	void AABB::Transform(const Matrix4& mat)
	{
		Vector3 edges[8];
		GetEdges(edges);

        auto v0 = mat * Vector4(GetCenter(), 1.0f);
        
        minPoint = maxPoint = Vector3(v0.x, v0.y, v0.z);

		for (size_t i = 0U; i < 8; ++i)
		{
            auto v = mat * Vector4(edges[i], 1.0f);
            AddPoint(v.x, v.y, v.z);
		}
    }

	AABB AABB::GetTransformed(const Matrix4& mat) const
	{
		AABB result(minPoint, maxPoint);
		result.Transform(mat);
		return result;
	}

} // math

} // 
