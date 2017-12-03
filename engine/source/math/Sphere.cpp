#include "Sphere.h"

namespace sh
{

namespace math
{

	float Sphere::GetRadius() const { return radius; }
	void Sphere::SetRadius(float r) { radius = r; }

	const Vector3& Sphere::GetCentre() const { return centre; }
	void Sphere::SetCentre(const Vector3& c) { centre = c; }

} // math

} // sh