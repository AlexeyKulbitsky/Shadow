#include "math/Plane.h"

namespace sh
{

namespace math
{

	Plane::Plane() : normal(0.0f, 1.0f, 0.0f) { RecalculateD(Vector3(0.0f, 0.0f, 0.0f)); }
	Plane::Plane(const Vector3 & MPoint, const Vector3 & Normal)
		: normal(Normal) {
		RecalculateD(MPoint);
	}
	Plane::Plane(float px, float py, float pz, float nx, float ny, float nz)
		: normal(nx, ny, nz) {
		RecalculateD(Vector3(px, py, pz));
	}
	Plane::Plane(const Vector3 & point1, const Vector3 & point2, const Vector3 & point3)
	{
		SetPlane(point1, point2, point3);
	}
	Plane::Plane(const Vector3 & Normal, const float D) : normal(Normal), d(D) { }

	void Plane::SetPlane(const Vector3 & point, const Vector3 & nvector)
	{
		normal = nvector;
		RecalculateD(point);
	}

	void Plane::SetPlane(const Vector3 & nvect, float D)
	{
		normal = nvect;
		d = D;
	}

	void Plane::SetPlane(const Vector3 & point1, const Vector3 & point2, const Vector3 & point3)
	{
		// creates the plane from 3 memberpoints
		normal = (point2 - point1).Cross(point3 - point1);
		normal.Normalize();

		RecalculateD(point1);
	}

	Plane& Plane::operator=(const Plane& p) { normal = p.normal; d = p.d; return *this; }

	void Plane::RecalculateD(const Vector3 & MPoint)
	{
		d = -MPoint.Dot(normal);
	}

	bool Plane::GetIntersectionWithLine(const Vector3 & linePoint, const Vector3 & lineVect, Vector3 & outIntersection) const
	{
		float t2 = normal.Dot(lineVect);

		if (t2 == 0.0f)
			return false;

		float t = -(normal.Dot(linePoint) + d) / t2;
		outIntersection = linePoint + (lineVect * t);
		return true;
	}

	float Plane::GetKnownIntersectionWithLine(const Vector3 & linePoint1, const Vector3 & linePoint2) const
	{
		Vector3 vect = linePoint2 - linePoint1;
		float t2 = normal.Dot(vect);
		return -((normal.Dot(linePoint1) + d) / t2);
	}

	float Plane::GetDistanceTo(const Vector3 & point) const
	{
		return point.Dot(normal) + d;
	}

} // math

} // sh