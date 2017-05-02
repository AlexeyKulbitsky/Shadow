#ifndef SHADOW_FUNCTIONS_RAY_INTERSECT_INCLUDE
#define SHADOW_FUNCTIONS_RAY_INTERSECT_INCLUDE

#include "Vector3.h"
#include "Plane.h"
#include "FunctionsCommon.h"

namespace sh
{
	namespace math
	{
		template<class T>
		inline T signOf(T tmp)
		{
			return tmp >= (T)0.0f ? (T)1.0f : (T)-1.0f;
		}

		template<class T>
		inline int QuadraticEquation(T a, T b, T c, T& x1, T& x2)
		{
			T q = b * b - 4 * a * c;
			if (q >= 0.0f)
			{
				T sq = Sqrt<T>(q);
				T d = 1.0f / (2.0f * a);
				x1 = (-b + sq) * d;
				x2 = (-b - sq) * d;

				return (Abs<T>(x1 - x2) < (T)1e-06) ? 1 : 2; // real roots
			}
			else
			{
				return 0;//complex roots
			}
		}

		template<class T>
		inline bool RayIntersectPlane(const Vector3<T>& origin, const Vector3<T>& dir, const Plane<T>& plane, T& t)
		{
			// Solution: t = -(p dot N + d) / (N dot dir)
			T denum = plane.normal.Dot(dir);

			// parallel to plane
			// if (denum!=0.0f) // cos 0 = 90
			if (Abs(denum) < (T)1e-6) // cos 0 = 90
			{
				return false;
			}
			
			T num = plane.GetDistanceTo(origin);
			t = -num / denum;
			return true;
		}

		template<class T>
		inline int RayIntersectSphere(const Vector3<T>& rayOrigin, const Vector3<T>& rayDirection, const Vector3<T>& sphereCenter, T radius, T& t0, T& t1)
		{
			// christer ericson code
			Vector3<T> m = rayOrigin - sphereCenter;
			T b = m.Dot(rayDirection);
			T c = m.Dot(m) - radius * radius;

			// exit if ray origin outside sphere ( c > 0) and r pointing away from sphere (b > 0)
			// if (c > 0.0f && b > 0.0f) return 0;

			T discr = b*b - c;
			if (discr < (T)0)
			{
				return 0;
			}

			T sqdiscr = Sqrt(discr);
			t0 = -b - sqdiscr;
			t1 = -b + sqdiscr;
			return 2;
		}

		template<class T>
		inline bool RayIntersectDisk(
			const Vector3<T>& rayOrigin, const Vector3<T>& rayDir,
			const Vector3<T>& circleCenter, const Vector3<T>& circleNormal, T circleRadius,
			T& t)
		{
			Plane<T> plane(circleNormal, circleCenter);
			T tt;
			
			if (!RayIntersectPlane(rayOrigin, rayDir, plane, tt))
			{
				return false;
			}

			Vector3<T> dist = circleCenter - (rayOrigin + rayDir * tt);

			if (dist.GetLengthSquare() < circleRadius * circleRadius)
			{
				t = tt;
				return true;
			}

			return false;
		}

		template<class T>
		inline int rayIntersectCylinder(
			const Vector3<T>& p, const Vector3<T>& d,
			T length, T radius,
			T* t, bool intersectDisks)
		{
			T hh = length / 2;
			T r2 = radius * radius;
			// check if ray is parallel to cylinder axis
			if (Abs(d.z) >= (T)0.999f)
			{
				if (!intersectDisks)
				{
					return 0;
				}
				// The line is parallel to the cylinder axis.  Determine if the line
				// intersects the cylinder end disks.

				T c = r2 - p.x*p.x - p.y*p.y;
				if (c < 0.0f)
				{
					// line outside the cylinder, no intersection
					return 0;
				}
				// line intersects the cylinder end disks
				if (d.z > 0.0f)
				{
					t[0] = -p.z - hh;
					t[1] = -p.z + hh;
				}
				else
				{
					t[0] = p.z - hh;
					t[1] = p.z + hh;
				}
				return 2;
			}
			// check cylinder walls
			// x^2 + y^2 = r^2
			// (Px+t*Dx)^2 + (Py+t*Dy)^2 = r^2
			// (Px^2 + t^2Dx^2 + 2*Px*t*Dx) + ... = r^2
			// t2*(Dx^2 + Dy^2) + t*(2*Px*Dx + 2*Py*Dy) + Px^2 + Py^2 - r^2 = 0;

			T a = d.x*d.x + d.y*d.y;
			T b = 2.0f*(p.x*d.x + p.y*d.y);
			T c = (p.x*p.x + p.y*p.y) - r2;
			T t0, t1;
			int ret = QuadraticEquation<T>(a, b, c, t0, t1);
			if (ret == 0)
			{
				return 0;
			}
			// check z bounds
			T z0 = p.z + d.z*t0;
			T z1 = p.z + d.z*t1;
			ret = 0;
			if (Abs(z0) < hh)
			{
				t[ret++] = t0;
			}
			else if (intersectDisks)
			{
				if (RayIntersectDisk<T>(p, d, Vector3<T>(0, 0, signOf(z0)*hh), Vector3<T>(0, 0, 1), radius, t0))
				{
					t[ret++] = t0;
				}
			}
			if (Abs(z1) < hh)
			{
				t[ret++] = t1;
			}
			else if (intersectDisks)
			{
				if (RayIntersectDisk<T>(p, d, Vector3<T>(0, 0, signOf(z1)*hh), Vector3<T>(0, 0, 1), radius, t1))
				{
					t[ret++] = t1;
				}
			}
			if (ret == 2)
			{
				if (t[0] > t[1]) std::swap(t[0], t[1]);
			}
			return ret;
		}
	}
}

#endif