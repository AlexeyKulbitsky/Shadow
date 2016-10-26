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
		inline int quadraticEquation(T a, T b, T c, T& x1, T& x2)
		{
			T q = b*b - 4 * a*c;
			if (q >= 0.0f)
			{
				T sq = math::sqrt<T>(q);
				T d = 1.0f / (2.0f*a);
				x1 = (-b + sq) * d;
				x2 = (-b - sq) * d;

				return (math::abs<T>(x1 - x2) < (T)1e-06) ? 1 : 2; // real roots
			}
			else
			{
				return 0;//complex roots
			}
		}

		template<class T>
		inline bool rayIntersectPlane(const math::vec3<T>& origin, const math::vec3<T>& dir, const math::plane<T>& plane, T& t)
		{
			// Solution: t = -(p dot N + d) / (N dot dir)
			T denum = math::dot(plane.normal, dir);

			// parallel to plane
			// if (denum!=0.0f) // cos 0 = 90
			if (math::abs(denum) < (T)1e-6) // cos 0 = 90
			{
				return false;
			}

			T num = plane.getDistanceTo(origin);
			t = -num / denum;
			return true;
		}

		template<class T>
		inline int rayIntersectAABB(const math::vec3<T>& rayOrigin, const math::vec3<T>& rayDir, const math::vec3<T>& boxCenter, const math::vec3<T>& boxHalfsize, T& tmin, T& tmax)
		{
			//http://www.cs.utah.edu/~awilliam/box/
			T tymin, tymax, tzmin, tzmax;
			math::vec3<T> bounds[2] = { boxCenter - boxHalfsize, boxCenter + boxHalfsize };
			int sign[3];
			math::vec3<T> inv_direction;
			inv_direction.x = 1.0f / rayDir.x;
			inv_direction.y = 1.0f / rayDir.y;
			inv_direction.z = 1.0f / rayDir.z;
			sign[0] = (inv_direction.x < 0);
			sign[1] = (inv_direction.y < 0);
			sign[2] = (inv_direction.z < 0);

			tmin = (bounds[sign[0]].x - rayOrigin.x) * inv_direction.x;
			tmax = (bounds[1 - sign[0]].x - rayOrigin.x) * inv_direction.x;

			tymin = (bounds[sign[1]].y - rayOrigin.y) * inv_direction.y;
			tymax = (bounds[1 - sign[1]].y - rayOrigin.y) * inv_direction.y;
			if ((tmin > tymax) || (tymin > tmax))
			{
				return 0;
			}
			if (tymin > tmin)
			{
				tmin = tymin;
			}
			if (tymax < tmax)
			{
				tmax = tymax;
			}
			tzmin = (bounds[sign[2]].z - rayOrigin.z) * inv_direction.z;
			tzmax = (bounds[1 - sign[2]].z - rayOrigin.z) * inv_direction.z;
			if ((tmin > tzmax) || (tzmin > tmax))
			{
				return 0;
			}
			if (tzmin > tmin)
			{
				tmin = tzmin;
			}
			if (tzmax < tmax)
			{
				tmax = tzmax;
			}
			return 2;
		}

		template<class T>
		inline int rayIntersectSphere(const math::vec3<T>& rayOrigin, const math::vec3<T>& rayDirection, const math::vec3<T>& sphereCenter, T radius, T& t0, T& t1)
		{
			// christer ericson code
			math::vec3<T> m = rayOrigin - sphereCenter;
			T b = math::dot(m, rayDirection);
			T c = math::dot(m, m) - radius*radius;

			// exit if ray origin outside sphere ( c > 0) and r pointing away from sphere (b > 0)
			// if (c > 0.0f && b > 0.0f) return 0;

			T discr = b*b - c;
			if (discr < 0.0f)
			{
				return 0;
			}

			T sqdiscr = math::sqrt(discr);
			t0 = -b - sqdiscr;
			t1 = -b + sqdiscr;
			return 2;
		}

		template<class T>
		inline int rayIntersectHemisphere(const math::vec3<T>& rayOrigin, const math::vec3<T>& rayDirection, const math::vec3<T>& sphereCenter, const math::vec3<T>& sphereNormal, T radius, T& t0, T& t1)
		{
			// christer ericson code
			math::vec3<T> m = rayOrigin - sphereCenter;
			T b = math::dot(m, rayDirection);
			T c = math::dot(m, m) - radius*radius;

			T discr = b*b - c;
			if (discr < 0.0f)
			{
				return 0;
			}

			T sqdiscr = math::sqrt(discr);
			int ret = 0;
			t0 = -b - sqdiscr;
			t1 = -b + sqdiscr;
			if (math::dot(m + rayDirection*t0, sphereNormal) > 0.0f)
			{
				ret++;
			}
			if (math::dot(m + rayDirection*t1, sphereNormal) > 0.0f)
			{
				if (ret == 0) t0 = t1;
				ret++;
			}
			return ret;
		}

		template<class T>
		inline bool rayIntersectDisk(
			const math::vec3<T>& rayOrigin, const math::vec3<T>& rayDir,
			const math::vec3<T>& circleCenter, const math::vec3<T>& circleNormal, T circleRadius,
			T& t)
		{
			math::plane<T> plane(circleNormal, circleCenter);
			T tt;
			if (!rayIntersectPlane(rayOrigin, rayDir, plane, tt))
			{
				return false;
			}

			math::vec3<T> dist = circleCenter - (rayOrigin + rayDir * tt);

			if (dist.getLengthSq() < circleRadius*circleRadius)
			{
				t = tt;
				return true;
			}

			return false;
		}

		template<class T>
		inline int rayIntersectCylinder(
			const math::vec3<T>& p, const math::vec3<T>& d,
			T length, T radius,
			T* t, bool intersectDisks)
		{
			T hh = length / 2;
			T r2 = radius*radius;
			// check if ray is parallel to cylinder axis
			if (math::abs(d.z) >= (T)0.999f)
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
			int ret = quadraticEquation<T>(a, b, c, t0, t1);
			if (ret == 0)
			{
				return 0;
			}
			// check z bounds
			T z0 = p.z + d.z*t0;
			T z1 = p.z + d.z*t1;
			ret = 0;
			if (math::abs(z0) < hh)
			{
				t[ret++] = t0;
			}
			else if (intersectDisks)
			{
				if (rayIntersectDisk<T>(p, d, math::vec3<T>(0, 0, signOf(z0)*hh), math::vec3<T>(0, 0, 1), radius, t0))
				{
					t[ret++] = t0;
				}
			}
			if (math::abs(z1) < hh)
			{
				t[ret++] = t1;
			}
			else if (intersectDisks)
			{
				if (rayIntersectDisk<T>(p, d, math::vec3<T>(0, 0, signOf(z1)*hh), math::vec3<T>(0, 0, 1), radius, t1))
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

		template<class T>
		inline int rayIntersectCapsule(const math::vec3<T>& rayOrigin, const math::vec3<T>& rayDir, T height, T radius, T& t0, T& t1)
		{
			// test caps first
			math::vec3<T> capCenter0(0, 0, height / 2);
			math::vec3<T> capCenter1(0, 0, -height / 2);
			T ts[4];
			int ret0 = rayIntersectHemisphere<T>(rayOrigin, rayDir, capCenter0, math::vec3<T>(0, 0, 1), radius, ts[0], ts[1]);
			int ret1 = rayIntersectHemisphere<T>(rayOrigin, rayDir, capCenter1, math::vec3<T>(0, 0, -1), radius, ts[2], ts[3]);
			if (ret0 > 0 && ret1 > 0)
			{
				//ray intersects both caps
				t0 = math::abs(ts[0]) > math::abs(ts[1]) ? ts[0] : ts[1];
				t1 = math::abs(ts[2]) > math::abs(ts[3]) ? ts[2] : ts[3];
				return 2;
			}
			// test walls
			T twall[2];
			int ret2 = rayIntersectCylinder<T>(rayOrigin, rayDir, height, radius, twall, false);
			if (ret2 == 0)// ray does not collide with the walls
			{
				// check cap collisions
				if (ret0 > 0)
				{
					t0 = ts[0];
					t1 = ts[1];
					return ret0;
				}
				else if (ret1 > 0)
				{
					t0 = ts[2];
					t1 = ts[3];
					return ret1;
				}
				return 0;
			}
			else if (ret2 > 1)
			{
				// ray crosses the cylinder wall
				t0 = twall[0];
				t1 = twall[1];
				return 2;
			}

			// if there is one collision point with the wall, it could have one collision point with the caps
			//PASSERT(ret2==1);
			if (ret0 + ret1 == 0)
			{
				t0 = twall[0];
				return 1;
			}
			t0 = (ret0 > 0) ? ts[0] : ts[2];
			t1 = twall[0];
			if (t0 > t1)
			{
				std::swap(t0, t1);
			}

			return 2;
		}

	}
}