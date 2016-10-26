namespace sh
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////
		// constructors
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		triangle3<T>::triangle3() {}

		template <typename T>
		triangle3<T>::triangle3(vec3<T> v1, vec3<T> v2, vec3<T> v3) : pointA(v1), pointB(v2), pointC(v3) {}

		///////////////////////////////////////////////////////////////////////////////
		// methods
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		bool triangle3<T>::operator==(const triangle3<T>& other) const
		{
			return other.pointA == pointA && other.pointB == pointB && other.pointC == pointC;
		}

		template <typename T>
		bool triangle3<T>::operator!=(const triangle3<T>& other) const
		{
			return !(*this == other);
		}

		template <typename T>
		bool triangle3<T>::isTotalInsideBox(const aabb<T>& box) const
		{
			return (box.isPointInside(pointA) &&
				box.isPointInside(pointB) &&
				box.isPointInside(pointC));
		}

		template <typename T>
		vec3<T> triangle3<T>::closestPointOnTriangle(const vec3<T>& p) const
		{
			const vec3<T> rab = line3<T>(pointA, pointB).getClosestPoint(p);
			const vec3<T> rbc = line3<T>(pointB, pointC).getClosestPoint(p);
			const vec3<T> rca = line3<T>(pointC, pointA).getClosestPoint(p);

			const T d1 = math::distance(rab, p);
			const T d2 = math::distance(rbc, p);
			const T d3 = math::distance(rca, p);

			if (d1 < d2)
				return d1 < d3 ? rab : rca;

			return d2 < d3 ? rbc : rca;
		}

		template <typename T>
		bool triangle3<T>::isPointInside(const vec3<T>& p) const
		{
			return (isOnSameSide(p, pointA, pointB, pointC) &&
				isOnSameSide(p, pointB, pointA, pointC) &&
				isOnSameSide(p, pointC, pointA, pointB));
		}

		template <typename T>
		bool triangle3<T>::isPointInsideFast(const vec3<T>& p) const
		{
			const vec3<T> f = pointB - pointA;
			const vec3<T> g = pointC - pointA;

			const float a = math::dot(f, f);
			const float b = math::dot(f, g);
			const float c = math::dot(g, g);

			const vec3<T> vp = p - pointA;
			const float d = math::dot(vp, f);
			const float e = math::dot(vp, g);

			float x = (d*c) - (e*b);
			float y = (e*a) - (d*b);
			const float ac_bb = (a*c) - (b*b);
			float z = x + y - ac_bb;

#define IR(x) ((uint32_t&)(x))
			// return sign(z) && !(sign(x)||sign(y))
			return (((IR(z)) & ~((IR(x)) | (IR(y)))) & 0x80000000) != 0;
#undef IR
		}

		template <typename T>
		bool triangle3<T>::getIntersectionWithLimitedLine(const line3<T>& line, vec3<T>& outIntersection) const
		{
			return getIntersectionWithLine(line.start,
				line.getVector(), outIntersection) &&
				line.isPointBetweenStartAndEnd(outIntersection);
			//outIntersection.isBetweenPoints(line.start, line.end)

		}

		template <typename T>
		bool triangle3<T>::getIntersectionWithLine(const vec3<T>& linePoint, const vec3<T>& lineVect, vec3<T>& outIntersection) const
		{
			if (getIntersectionOfPlaneWithLine(linePoint, lineVect, outIntersection))
				return isPointInside(outIntersection);

			return false;
		}

		template <typename T>
		bool triangle3<T>::getIntersectionWithLineFast(const vec3<T>& linePoint, const vec3<T>& lineVect, vec3<T>& outIntersection) const
		{
			T t, u, v;
			bool ret = intersectRay(linePoint, lineVect, false, t, u, v);
			if (ret)
			{
				// calculate intersection point
				outIntersection = linePoint + t*lineVect;
			}
			return ret;
		}

		template <typename T>
		bool triangle3<T>::intersectRay(const vec3<T>& linePoint, const vec3<T>& lineVect, bool cullbackface, T& t, T& u, T& v) const
		{
			// every point inside a triangle is given by the explicit formula
			//	t(u,v) = o + t*d = (1-u-v)v0 + u.v1 + v.v2)
			T det, inv_det;
			vec3<T> pvec, tvec, qvec;

			// find vectors for two edges sharing vert0
			const vec3<T>& edge1 = pointB - pointA;
			const vec3<T>& edge2 = pointC - pointA;

			// backface culling
			if (cullbackface && math::dot(math::cross(edge1, edge2), linePoint - pointA) < 0.0)
			{
				return false;
			}

			// begin calculating determinant - also used to calculate U parameter
			pvec = math::cross(lineVect, edge2);

			// if determinant is near zero, ray lies in plane of triangle
			det = math::dot(edge1, pvec);
			if (math::isZero(det))
				return false;
			inv_det = T(1) / det;

			// calculate distance from vert0 to ray origin
			tvec = linePoint - pointA;

			// calculate U parameter and test bounds
			u = math::dot(tvec, pvec) * inv_det;
			if (u < 0 || u > T(1))
				return false;

			// prepare to test V parameter 
			qvec = math::cross(tvec, edge1);

			// calculate V parameter and test bounds
			v = math::dot(lineVect, qvec) * inv_det;
			if (v < 0.0f || u + v > 1.0f)
				return false;

			// calculate t, ray intersects triangle
			t = math::dot(edge2, qvec) * inv_det;
			return true;
		}

		template <typename T>
		bool triangle3<T>::getIntersectionOfPlaneWithLine(const vec3<T>& linePoint,
			const vec3<T>& lineVect, vec3<T>& outIntersection) const
		{
			const vec3<T> normal = math::normalized(getNormal());
			T t2 = dot(normal, lineVect);
			if (isZero(t2))
				return false;

			T d = dot(pointA, normal);
			T t = -(dot(normal, linePoint) - d) / t2;
			outIntersection = linePoint + (lineVect * t);
			return true;
		}

		template <typename T>
		bool triangle3<T>::getIntersectionOfPlaneWithLineFast(const vec3<T>& linePoint,
			const vec3<T>& lineVect, vec3<T>& outIntersection) const
		{
			const vec3<T> normal = getNormal().normalize();
			T t2 = math::dot(normal, lineVect);

			if (math::isZero(t2))
				return false;

			T d = math::dot(pointA, normal);
			T t = -(math::dot(normal, linePoint) - d) / t2;
			outIntersection = linePoint + (lineVect * t);
			return true;
		}

		template <typename T>
		vec3<T> triangle3<T>::getNormal() const
		{
			return cross(pointB - pointA, pointC - pointA);
		}

		template <typename T>
		bool triangle3<T>::isFrontFacing(const vec3<T>& lookDirection) const
		{
			const vec3<T> n = getNormal();
			n.normalize();
			const T d = math::dot(n, lookDirection);
			return F32_LOWER_EQUAL_0(d);
		}

		template <typename T>
		plane<T> triangle3<T>::getPlane() const
		{
			return plane<T>(pointA, pointB, pointC);
		}

		template <typename T>
		T triangle3<T>::getArea() const
		{
			return cross((pointB - pointA), pointC - pointA).getLength() * 0.5f;
		}

		template <typename T>
		T triangle3<T>::getAreaSq() const
		{
			return cross((pointB - pointA), pointC - pointA).getLengthSq() * 0.25f;
		}


		template <typename T>
		void triangle3<T>::set(const vec3<T>& a, const vec3<T>& b, const vec3<T>& c)
		{
			pointA = a;
			pointB = b;
			pointC = c;
		}

		template <typename T>
		bool triangle3<T>::isOnSameSide(const vec3<T>& p1, const vec3<T>& p2,
			const vec3<T>& a, const vec3<T>& b) const
		{
			vec3<T> bminusa = b - a;
			vec3<T> cp1 = math::cross(bminusa, p1 - a);
			vec3<T> cp2 = math::cross(bminusa, p2 - a);
			return (math::dot(cp1, cp2) >= 0.0f);
		}

	}
}