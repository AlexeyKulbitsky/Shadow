namespace sh
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////
		// constructors
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		aabb2<T>::aabb2() : minPoint(-1, -1), maxPoint(1, 1)
		{
		}

		template <typename T>
		aabb2<T>::aabb2(const vec2<T>& minp, const vec2<T>& maxp) : minPoint(minp), maxPoint(maxp)
		{
		}

		template <typename T>
		aabb2<T>::aabb2(const vec2<T>& init) : minPoint(init), maxPoint(init)
		{
		}

		template <typename T>
		aabb2<T>::aabb2(T minx, T miny, T maxx, T maxy) : minPoint(minx, miny), maxPoint(maxx, maxy)
		{
		}

		///////////////////////////////////////////////////////////////////////////////
		// methods
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		void aabb2<T>::addInternalPoint(const vec2<T>& p)
		{
			addInternalPoint(p.x, p.y);
		}

		template <typename T>
		void aabb2<T>::addInternalBox(const aabb2<T>& b)
		{
			addInternalPoint(b.maxPoint);
			addInternalPoint(b.minPoint);
		}

		template <typename T>
		void aabb2<T>::addInternalBox(const mat3<T>& mat, const aabb2<T>& b)
		{
			aabb2<T> tmp;
			tmp.reset(mat, b);
			addInternalBox(tmp);
		}

		template <typename T>
		void aabb2<T>::reset(T x, T y)
		{
			maxPoint.set(x, y);
			minPoint = maxPoint;
		}

		template <typename T>
		void aabb2<T>::reset(const aabb2<T>& initValue)
		{
			*this = initValue;
		}

		template <typename T>
		void aabb2<T>::reset(const vec2<T>& minp, const vec2<T>& maxp)
		{
			minPoint = minp;
			maxPoint = maxp;
		}

		template <typename T>
		void aabb2<T>::reset(const vec2<T>& initValue)
		{
			maxPoint = initValue;
			minPoint = initValue;
		}

		template <typename T>
		void aabb2<T>::reset(const mat3<T>& mat, const aabb2<T>& b)
		{
			// compute projected radius interval
			vec2<T> center = b.getCenter();
			vec2<T> hs = b.getExtent() * 0.5f;
			vec2<T> tmp;
			tmp.x = math::abs<T>(mat.m[0])*hs.x + math::abs<T>(mat.m[1])*hs.y;
			tmp.y = math::abs<T>(mat.m[3])*hs.x + math::abs<T>(mat.m[4])*hs.y;

			center = math::transform(mat, center);

			maxPoint = (center + tmp);
			minPoint = (center - tmp);
		}

		template <typename T>
		vec2<T> aabb2<T>::getExtent() const
		{
			return maxPoint - minPoint;
		}

		template <typename T>
		T aabb2<T>::getRadius() const
		{
			return getExtent().getLength() * 0.5f;
		}

		template <typename T>
		T aabb2<T>::getRadiusFast() const
		{
			float radiusSq = getExtent().getLengthSq() * 0.25f;
			return math::sqrt(radiusSq);
		}

		template <typename T>
		T aabb2<T>::getRadiusSq() const
		{
			return getExtent().getLengthSq() * 0.25f;
		}

		template <typename T>
		T aabb2<T>::getInnerRadius() const
		{
			float dx = maxPoint.x - minPoint.x;
			float dy = maxPoint.y - minPoint.y;
			float max = math::max(dx, dy);
			return max * 0.5f;
		}

		template <typename T>
		T aabb2<T>::getInnerRadiusSq() const
		{
			float r = getInnerRadius();
			return r * r;
		}

		template <typename T>
		void aabb2<T>::getEdges(vec2<T> edges[4]) const
		{
			const vec2<T> middle = getCenter();
			const vec2<T> diag = middle - maxPoint;

			/*
			Edges are stored in this way:
			Hey, am I an ascii artist, or what? :) niko.
			1---------2
			|         |
			|         |
			|         |
			0---------3
			*/

			edges[0] = vec2<T>(middle.x - diag.x, middle.y - diag.y);
			edges[1] = vec2<T>(middle.x - diag.x, middle.y + diag.y);
			edges[2] = vec2<T>(middle.x + diag.x, middle.y + diag.y);
			edges[3] = vec2<T>(middle.x + diag.x, middle.y - diag.y);
		}

		template <typename T>
		bool aabb2<T>::isEmpty() const
		{
			return math::equals(minPoint, maxPoint);
		}

		template <typename T>
		void aabb2<T>::repair()
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
		}

		template <typename T>
		aabb2<T> aabb2<T>::getInterpolated(const aabb2<T>& other, float d) const
		{
			float inv = 1.0f - d;
			return aabb2<T>((other.minPoint*inv) + (minPoint*d),
				(other.maxPoint*inv) + (maxPoint*d));
		}

		template <typename T>
		vec2<T> aabb2<T>::getCenter() const
		{
			return (minPoint + maxPoint) * 0.5f;
		}

		template <typename T>
		T aabb2<T>::getArea() const
		{
			const vec2<T> e = getExtent();
			return e.x * e.y;
		}

		///////////////////////////////////////////////////////////////////////////////
		// containment
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		bool aabb2<T>::isPointInside(const vec2<T>& p) const
		{
			return (
				p.x >= minPoint.x  &&  p.x <= maxPoint.x  &&
				p.y >= minPoint.y  &&  p.y <= maxPoint.y);
		}

		template <typename T>
		bool aabb2<T>::isPointTotalInside(const vec2<T>& p) const
		{
			return (
				p.x > minPoint.x  &&  p.x < maxPoint.x  &&
				p.y > minPoint.y  &&  p.y < maxPoint.y);
		}

		template <typename T>
		void aabb2<T>::addInternalPoint(T x, T y)
		{
			if (x>maxPoint.x) maxPoint.x = x;
			if (y>maxPoint.y) maxPoint.y = y;

			if (x<minPoint.x) minPoint.x = x;
			if (y<minPoint.y) minPoint.y = y;
		}



		///////////////////////////////////////////////////////////////////////////////
		// intersection
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		bool aabb2<T>::intersectsWithBox(const aabb2<T>& other) const
		{
			//return (minPoint <= other.maxPoint && maxPoint >= other.minPoint);
			return minPoint.x <= other.maxPoint.x && maxPoint.x >= other.minPoint.x
				&& minPoint.y <= other.maxPoint.y && maxPoint.y >= other.minPoint.y;
		}

		template <typename T>
		bool aabb2<T>::isFullInside(const aabb2<T>& other) const
		{
			//return minPoint >= other.minPoint && maxPoint <= other.maxPoint;
			return minPoint.x >= other.minPoint.x && maxPoint.x <= other.maxPoint.x
				&& minPoint.y >= other.minPoint.y && maxPoint.y <= other.maxPoint.y;
		}

		template <typename T>
		bool aabb2<T>::intersectsWithLine(const vec2<T>& linemiddle, const vec2<T>& linevect, T halflength) const
		{
			const vec2<T> e = getExtent() * (T)0.5;
			const vec2<T> t = getCenter() - linemiddle;

			if (
				(math::abs(t.x) > e.x + halflength * math::abs(linevect.x)) ||
				(math::abs(t.y) > e.y + halflength * math::abs(linevect.y)))
				return false;

			T r = e.x * (T)math::abs(linevect.y) + e.y * (T)math::abs(linevect.x);
			if (math::abs(t.x*linevect.y - t.y*linevect.x) > r)
				return false;

			return true;
		}

		template <typename T>
		bool aabb2<T>::intersectsWithLine(const line2<T>& line) const
		{
			return intersectsWithLine(line.getMiddle(),
				line.getVectorNormalized(),
				line.getLength() * 0.5f);
		}

		template <typename T>
		bool aabb2<T>::intersectsWithRay(const line2<T>& ray, vec2<T>& outIntersectionNear, vec2<T>& outIntersectionFar) const
		{
			const vec2<T>& minBounds = minPoint;
			const vec2<T>& maxBounds = maxPoint;

			const vec2<T>& rayOrigin = ray.start;
			const vec2<T>& rayDirection = ray.getVectorNormalized();

			T tnear = -std::numeric_limits<T>::max();
			T tfar = std::numeric_limits<T>::max();

			for (size_t i = 0; i < 2; ++i)
			{
				T rayOriginI = rayOrigin[i];
				T rayDirectionI = rayDirection[i];
				T minBoundsI = minBounds[i];
				T maxBoundsI = maxBounds[i];
				if (rayDirectionI == 0) // the ray is parallel to the planes so: 
				{
					if (rayOriginI < minBoundsI || rayOriginI > maxBoundsI) //origin not between planes
						return false;
				}
				else
				{
					T t1 = (minBoundsI - rayOriginI) / rayDirectionI; // time at which ray intersects minimum i plane
					T t2 = (maxBoundsI - rayOriginI) / rayDirectionI; // time at which ray intersects maximum i plane
					if (t1 > t2)
						std::swap(t1, t2); // make t1 the lower value
					if (t1 > tnear)
						tnear = t1; // update tnear
					if (t2 < tfar)
						tfar = t2; // update tfar
					if (tnear > tfar) //box is missed so return FALSE 
						return false;
					if (tfar < 0) //box is behind ray so return FALSE
						return false;
				}
			}

			outIntersectionNear = rayOrigin + rayDirection * tnear;
			outIntersectionFar = rayOrigin + rayDirection * tfar;
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////
		// operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		bool aabb2<T>::operator==(const aabb2<T>& other) const
		{
			return (minPoint == other.minPoint && other.maxPoint == maxPoint);
		}

		template <typename T>
		bool aabb2<T>::operator!=(const aabb2<T>& other) const
		{
			return !(*this == other);
		}

	}
}