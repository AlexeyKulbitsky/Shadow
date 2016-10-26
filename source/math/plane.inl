namespace sh
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////
		// constructors
		///////////////////////////////////////////////////////////////////////////////
		template <class T>
		inline plane<T>::plane() : normal(0, 0, 1), d(0)
		{
		}
		template <class T>
		inline plane<T>::plane(const math::vec3<T>& ipoint, const math::vec3<T>& inormal)
		{
			setPlane(ipoint, inormal);
		}

		template <class T>
		inline plane<T>::plane(T px, T py, T pz, T nx, T ny, T nz) : normal(nx, ny, nz)
		{
			recalculateD(math::vec3<T>(px, py, pz));
		}

		template <class T>
		inline plane<T>::plane(const math::vec3<T>& point1, const math::vec3<T>& point2, const math::vec3<T>& point3)
		{
			setPlane(point1, point2, point3);
		}

		///////////////////////////////////////////////////////////////////////////////
		// operators
		///////////////////////////////////////////////////////////////////////////////

		template <class T>
		inline bool plane<T>::operator==(const plane<T>& other) const
		{
			return math::equals(d, other.d) && math::equals(normal, other.normal);
		}

		template <class T>
		inline bool plane<T>::operator!=(const plane<T>& other) const
		{
			return !(*this == other);
		}

		///////////////////////////////////////////////////////////////////////////////
		// methods
		///////////////////////////////////////////////////////////////////////////////

		template <class T>
		inline void plane<T>::setPlane(const math::vec3<T>& ipoint, const math::vec3<T>& inormal)
		{
			normal = inormal;
			recalculateD(ipoint);
		}

		template <class T>
		inline void plane<T>::setPlane(const math::vec3<T>& inormal, T id)
		{
			normal = inormal;
			d = id;
		}

		template <class T>
		inline void plane<T>::setPlane(const math::vec3<T>& point1, const math::vec3<T>& point2, const math::vec3<T>& point3)
		{
			// creates the plane from 3 memberpoints
			normal = math::cross(point2 - point1, point3 - point1);
			normal.normalize();
			recalculateD(point1);
		}


		template <class T>
		inline void plane<T>::recalculateD(const math::vec3<T>& ipoint)
		{
			d = -math::dot(ipoint, normal);
		}

		template <class T>
		inline math::vec3<T> plane<T>::getMemberPoint() const
		{
			return normal * -d;
		}


		template <class T>
		inline bool plane<T>::isFrontFacing(const math::vec3<T>& lookDirection) const
		{
			const T d = math::dot(normal, lookDirection);
			return F32_LOWER_EQUAL_0(d);
		}

		template <class T>
		inline T plane<T>::getDistanceTo(const math::vec3<T>& point) const
		{
			return math::dot(point, normal) + d;
		}

		template <class T>
		math::vec3<T> plane<T>::projectPoint(const math::vec3<T>& point) const
		{
			T dist = getDistanceTo(point);
			math::vec3<T> result = point - normal * dist;
			return result;
		}

		template <class T>
		plane<T>& plane<T>::invert()
		{
			plane<T> tmp = *this;
			tmp.normal = -normal;
			tmp.d = -math::dot(tmp.normal, getMemberPoint());
			*this = tmp;
			return *this;
		}

		///////////////////////////////////////////////////////////////////////////////
		// intersection methods
		///////////////////////////////////////////////////////////////////////////////

		template <class T>
		inline bool plane<T>::getIntersectionWithLine(const math::vec3<T>& linePoint, const math::vec3<T>& lineVect, math::vec3<T>& outIntersection) const
		{
			T t2 = math::dot(normal, lineVect);

			if (t2 == 0)
				return false;

			T t = -(math::dot(normal, linePoint) + d) / t2;
			outIntersection = linePoint + (lineVect * t);
			return true;
		}

		template <class T>
		inline float plane<T>::getKnownIntersectionWithLine(const math::vec3<T>& linePoint1, const math::vec3<T>& linePoint2) const
		{
			math::vec3<T> vect = linePoint2 - linePoint1;
			T t2 = (float)math::dot(normal, vect);
			return (float)-((math::dot(normal, linePoint1) + d) / t2);
		}

		template <class T>
		inline bool plane<T>::getIntersectionWithLimitedLine(const math::vec3<T>& linePoint1, const math::vec3<T>& linePoint2, math::vec3<T>& outIntersection) const
		{
			//PERE: check that this is correct. Old code wasn't compiling.
			assert(0);

			return getIntersectionWithLine(linePoint1, linePoint2 - linePoint1, outIntersection) && line3<T>(linePoint1, linePoint2).isBetweenPoints(outIntersection);
		}

		template <class T>
		inline RelationEnum plane<T>::classifyPointRelation(const math::vec3<T>& point) const
		{
			const T dist = math::dot(normal, point) + d;

			if (dist < -std::numeric_limits<T>::epsilon())
				return k_relBack;

			if (dist > std::numeric_limits<T>::epsilon())
				return k_relFront;

			return k_relPlanar;
		}

		template <class T>
		inline bool plane<T>::existsIntersection(const plane<T>& other) const
		{
			math::vec3<T> cross = math::cross(other.normal, normal);
			return cross.getLength() > std::numeric_limits<T>::epsilon();
		}

		template <class T>
		inline bool plane<T>::getIntersectionWithPlane(const plane<T>& other, math::vec3<T>& outLinePoint, math::vec3<T>& outLineVect) const
		{
			const T fn00 = normal.getLength();
			const T fn01 = math::dot(normal, other.normal);
			const T fn11 = other.normal.getLength();
			const double det = fn00*fn11 - fn01*fn01;

			if (math::abs(det) < std::numeric_limits<T>::epsilon())
				return false;

			const double invdet = 1.0 / det;
			const double fc0 = (fn11*-d + fn01*other.d) * invdet;
			const double fc1 = (fn00*-other.d + fn01*d) * invdet;

			outLineVect = math::cross(normal, other.normal);
			outLinePoint = normal*(T)fc0 + other.normal*(T)fc1;
			return true;
		}

		template <class T>
		inline bool plane<T>::getIntersectionWithPlanes(const plane<T>& o1, const plane<T>& o2, math::vec3<T>& outPoint) const
		{
			math::vec3<T> linePoint, lineVect;
			if (getIntersectionWithPlane(o1, linePoint, lineVect))
				return o2.getIntersectionWithLine(linePoint, lineVect, outPoint);

			return false;
		}

		template <class T>
		inline void plane<T>::translate(const math::vec3<T>& offset)
		{
			math::vec3<T> pp = getMemberPoint();
			setPlane(pp + offset, normal);
		}

		///////////////////////////////////////////////////////////////////////////////
		// overlap methods
		///////////////////////////////////////////////////////////////////////////////

		template <class T>
		int32_t plane<T>::overlap(const math::vec3<T>& boxCenter, math::vec3<T>& boxHalfsize) const
		{
			const T tmp[3] = {
				math::abs(boxHalfsize.x * normal.x),
				math::abs(boxHalfsize.y * normal.y),
				math::abs(boxHalfsize.z * normal.z)
			};

			float radius = tmp[0] + tmp[1] + tmp[2];
			float distance = getDistanceTo(boxCenter);
			if (distance <= -radius) return 0;
			else if (distance <= radius) return 2;
			else return 1;
		}

	}
}