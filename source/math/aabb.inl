namespace sh
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////
		// constructors
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		aabb<T>::aabb() : minPoint(-1, -1, -1), maxPoint(1, 1, 1)
		{
		}

		template <typename T>
		aabb<T>::aabb(const vec3<T>& minp, const vec3<T>& maxp) : minPoint(minp), maxPoint(maxp)
		{
		}

		template <typename T>
		aabb<T>::aabb(const vec3<T>& init) : minPoint(init), maxPoint(init)
		{
		}

		template <typename T>
		aabb<T>::aabb(T minx, T miny, T minz, T maxx, T maxy, T maxz) : minPoint(minx, miny, minz), maxPoint(maxx, maxy, maxz)
		{
		}

		///////////////////////////////////////////////////////////////////////////////
		// methods
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		void aabb<T>::addInternalPoint(const vec3<T>& p)
		{
			addInternalPoint(p.x, p.y, p.z);
		}

		template <typename T>
		void aabb<T>::addInternalBox(const aabb<T>& b)
		{
			addInternalPoint(b.maxPoint);
			addInternalPoint(b.minPoint);
		}

		template <typename T>
		void aabb<T>::addInternalBox(const mat4<T>& mat, const aabb<T>& b)
		{
			aabb<T> tmp;
			tmp.reset(mat, b);
			addInternalBox(tmp);
		}

		template <typename T>
		void aabb<T>::reset(T x, T y, T z)
		{
			maxPoint.set(x, y, z);
			minPoint = maxPoint;
		}

		template <typename T>
		void aabb<T>::reset(const aabb<T>& initValue)
		{
			*this = initValue;
		}

		template <typename T>
		void aabb<T>::reset(const vec3<T>& minp, const vec3<T>& maxp)
		{
			minPoint = minp;
			maxPoint = maxp;
		}

		template <typename T>
		void aabb<T>::reset(const vec3<T>& initValue)
		{
			maxPoint = initValue;
			minPoint = initValue;
		}

		template <typename T>
		void aabb<T>::reset(const mat4<T>& mat, const aabb<T>& b)
		{
			// compute projected radius interval
			vec3<T> center = b.getCenter();
			vec3<T> hs = b.getExtent() * 0.5f;
			vec3<T> tmp;
			tmp.x = math::abs<T>(mat.m[0])*hs.x + math::abs<T>(mat.m[1])*hs.y + math::abs<T>(mat.m[2])*hs.z;
			tmp.y = math::abs<T>(mat.m[4])*hs.x + math::abs<T>(mat.m[5])*hs.y + math::abs<T>(mat.m[6])*hs.z;
			tmp.z = math::abs<T>(mat.m[8])*hs.x + math::abs<T>(mat.m[9])*hs.y + math::abs<T>(mat.m[10])*hs.z;

			center = math::transform(mat, center);//mat * center;

			maxPoint = (center + tmp);
			minPoint = (center - tmp);
		}

		template <typename T>
		vec3<T> aabb<T>::getExtent() const
		{
			return maxPoint - minPoint;
		}

		template <typename T>
		T aabb<T>::getRadius() const
		{
			return getExtent().getLength() * 0.5f;
		}

		template <typename T>
		T aabb<T>::getRadiusFast() const
		{
			float radiusSq = getExtent().getLengthSq() * 0.25f;
			return math::sqrt(radiusSq);
		}

		template <typename T>
		T aabb<T>::getRadiusSq() const
		{
			return getExtent().getLengthSq() * 0.25f;
		}

		template <typename T>
		T aabb<T>::getInnerRadius() const
		{
			float dx = maxPoint.x - minPoint.x;
			float dy = maxPoint.y - minPoint.y;
			float dz = maxPoint.z - minPoint.z;
			float max = math::max(dx, dy, dz);
			return max * 0.5f;
		}

		template <typename T>
		T aabb<T>::getInnerRadiusSq() const
		{
			float r = getInnerRadius();
			return r * r;
		}

		template <typename T>
		void aabb<T>::getEdges(vec3<T> edges[8]) const
		{
			const vec3<T> middle = getCenter();
			const vec3<T> diag = middle - maxPoint;

			/*
			Edges are stored in this way:
			Hey, am I an ascii artist, or what? :) niko.
			/3--------/7
			/  |      / |
			/   |     /  |
			1---------5  |
			|   2- - -| -6
			|  /      |  /
			|/        | /
			0---------4/
			*/

			edges[0] = vec3<T>(middle.x + diag.x, middle.y + diag.y, middle.z + diag.z);
			edges[1] = vec3<T>(middle.x + diag.x, middle.y - diag.y, middle.z + diag.z);
			edges[2] = vec3<T>(middle.x + diag.x, middle.y + diag.y, middle.z - diag.z);
			edges[3] = vec3<T>(middle.x + diag.x, middle.y - diag.y, middle.z - diag.z);
			edges[4] = vec3<T>(middle.x - diag.x, middle.y + diag.y, middle.z + diag.z);
			edges[5] = vec3<T>(middle.x - diag.x, middle.y - diag.y, middle.z + diag.z);
			edges[6] = vec3<T>(middle.x - diag.x, middle.y + diag.y, middle.z - diag.z);
			edges[7] = vec3<T>(middle.x - diag.x, middle.y - diag.y, middle.z - diag.z);
		}

		template <typename T>
		bool aabb<T>::isEmpty() const
		{
			return math::equals(minPoint, maxPoint);
		}

		template <typename T>
		void aabb<T>::repair()
		{
			T t;

			if (minPoint.x > maxPoint.x)
			{
				t = minPoint.x; minPoint.x = maxPoint.x; maxPoint.x = t;
			}
			if (minPoint.y > maxPoint.y)
			{
				t = minPoint.y; minPoint.y = maxPoint.y; maxPoint.y = t;
			}
			if (minPoint.z > maxPoint.z)
			{
				t = minPoint.z; minPoint.z = maxPoint.z; maxPoint.z = t;
			}
		}

		template <typename T>
		aabb<T> aabb<T>::getInterpolated(const aabb<T>& other, float d) const
		{
			float inv = 1.0f - d;
			return aabb<T>((other.minPoint*inv) + (minPoint*d),
				(other.maxPoint*inv) + (maxPoint*d));
		}

		template <typename T>
		vec3<T> aabb<T>::getCenter() const
		{
			return (minPoint + maxPoint) * 0.5f;
		}

		template <typename T>
		T aabb<T>::getVolume() const
		{
			const vec3<T> e = getExtent();
			return e.x * e.y * e.z;
		}

		template <typename T>
		T aabb<T>::getArea() const
		{
			const vec3<T> e = getExtent();
			return 2 * (e.x*e.y + e.x*e.z + e.y*e.z);
		}

		///////////////////////////////////////////////////////////////////////////////
		// containment
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		bool aabb<T>::isPointInside(const vec3<T>& p) const
		{
			return (
				p.x >= minPoint.x  &&  p.x <= maxPoint.x  &&
				p.y >= minPoint.y  &&  p.y <= maxPoint.y  &&
				p.z >= minPoint.z  &&  p.z <= maxPoint.z);
		}

		template <typename T>
		bool aabb<T>::isPointTotalInside(const vec3<T>& p) const
		{
			return (
				p.x > minPoint.x  &&  p.x < maxPoint.x  &&
				p.y > minPoint.y  &&  p.y < maxPoint.y  &&
				p.z > minPoint.z  &&  p.z < maxPoint.z);
		}

		template <typename T>
		void aabb<T>::addInternalPoint(T x, T y, T z)
		{
			if (x>maxPoint.x) maxPoint.x = x;
			if (y>maxPoint.y) maxPoint.y = y;
			if (z>maxPoint.z) maxPoint.z = z;

			if (x<minPoint.x) minPoint.x = x;
			if (y<minPoint.y) minPoint.y = y;
			if (z<minPoint.z) minPoint.z = z;
		}



		///////////////////////////////////////////////////////////////////////////////
		// intersection
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		bool aabb<T>::intersectsWithBox(const aabb<T>& other) const
		{
			//return (minPoint <= other.maxPoint && maxPoint >= other.minPoint);
			return minPoint.x <= other.maxPoint.x && maxPoint.x >= other.minPoint.x
				&& minPoint.y <= other.maxPoint.y && maxPoint.y >= other.minPoint.y
				&& minPoint.z <= other.maxPoint.z && maxPoint.z >= other.minPoint.z;
		}

		template <typename T>
		bool aabb<T>::isFullInside(const aabb<T>& other) const
		{
			//return minPoint >= other.minPoint && maxPoint <= other.maxPoint;
			return minPoint.x >= other.minPoint.x && maxPoint.x <= other.maxPoint.x
				&& minPoint.y >= other.minPoint.y && maxPoint.y <= other.maxPoint.y
				&& minPoint.z >= other.minPoint.z && maxPoint.z <= other.maxPoint.z;
		}

		template <typename T>
		bool aabb<T>::intersectsWithLine(const vec3<T>& linemiddle, const vec3<T>& linevect, T halflength) const
		{
			const vec3<T> e = getExtent() * (T)0.5;
			const vec3<T> t = getCenter() - linemiddle;

			if (
				(math::abs(t.x) > e.x + halflength * math::abs(linevect.x)) ||
				(math::abs(t.y) > e.y + halflength * math::abs(linevect.y)) ||
				(math::abs(t.z) > e.z + halflength * math::abs(linevect.z)))
				return false;

			T r = e.y * (T)math::abs(linevect.z) + e.z * (T)math::abs(linevect.y);
			if (math::abs(t.y*linevect.z - t.z*linevect.y) > r)
				return false;

			r = e.x * (T)math::abs(linevect.z) + e.z * (T)math::abs(linevect.x);
			if (math::abs(t.z*linevect.x - t.x*linevect.z) > r)
				return false;

			r = e.x * (T)math::abs(linevect.y) + e.y * (T)math::abs(linevect.x);
			if (math::abs(t.x*linevect.y - t.y*linevect.x) > r)
				return false;

			return true;
		}

		template <typename T>
		bool aabb<T>::intersectsWithLine(const line3<T>& line) const
		{
			return intersectsWithLine(line.getMiddle(),
				line.getVectorNormalized(),
				line.getLength() * 0.5f);
		}

		template <typename T>
		bool aabb<T>::intersectsWithRay(const line3<T>& ray, float& outIntersectionNearDistance, float& outIntersectionFarDistance) const
		{
			const vec3<T>& minBounds = minPoint;
			const vec3<T>& maxBounds = maxPoint;

			const vec3<T>& rayOrigin = ray.start;
			const vec3<T>& rayDirection = ray.getVectorNormalized();

			T tnear = -std::numeric_limits<T>::max();
			T tfar = std::numeric_limits<T>::max();

			for (size_t i = 0; i < 3; ++i)
			{
				T rayOriginI = rayOrigin[i];
				T rayDirectionI = rayDirection[i];
				T minBoundsI = minBounds[i];
				T maxBoundsI = maxBounds[i];
				if (rayDirectionI == 0) // the ray is parallel to the planes so: 
				{
					if (rayOriginI < minBoundsI || rayOriginI > maxBoundsI) //origin not between planes
					{
						return false;
					}
				}
				else
				{
					T t1 = (minBoundsI - rayOriginI) / rayDirectionI; // time at which ray intersects minimum i plane
					T t2 = (maxBoundsI - rayOriginI) / rayDirectionI; // time at which ray intersects maximum i plane
					if (t1 > t2)
					{
						std::swap(t1, t2); // make t1 the lower value
					}
					if (t1 > tnear)
					{
						tnear = t1; // update tnear
					}
					if (t2 < tfar)
					{
						tfar = t2; // update tfar
					}
					if (tnear > tfar) //box is missed so return FALSE 
					{
						return false;
					}
					if (tfar < 0) //box is behind ray so return FALSE
					{
						return false;
					}
				}
			}

			outIntersectionNearDistance = tnear;
			outIntersectionFarDistance = tfar;

			return true;
		}

		template <typename T>
		bool aabb<T>::intersectsWithRay(const line3<T>& ray, vec3<T>& outIntersectionNear, vec3<T>& outIntersectionFar) const
		{
			float outIntersectionNearDistance;
			float outIntersectionFarDistance;
			bool result = intersectsWithRay(ray, outIntersectionNearDistance, outIntersectionFarDistance);

			if (result)
			{
				const vec3<T>& rayOrigin = ray.start;
				const vec3<T>& rayDirection = ray.getVectorNormalized();

				outIntersectionNear = rayOrigin + rayDirection * outIntersectionNearDistance;
				outIntersectionFar = rayOrigin + rayDirection * outIntersectionFarDistance;
			}

			return result;
		}

		///////////////////////////////////////////////////////////////////////////////
		// operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		bool aabb<T>::operator==(const aabb<T>& other) const
		{
			return (minPoint == other.minPoint && other.maxPoint == maxPoint);
		}

		template <typename T>
		bool aabb<T>::operator!=(const aabb<T>& other) const
		{
			return !(*this == other);
		}

	}
}