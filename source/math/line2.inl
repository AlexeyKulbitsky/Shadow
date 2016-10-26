namespace sh
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////
		// constructors
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		inline line2<T>::line2() : start(0, 0), end(1, 1)
		{
		}

		template <typename T>
		inline line2<T>::line2(T xa, T ya, T xb, T yb) : start(xa, ya), end(xb, yb)
		{

		}

		template <typename T>
		inline line2<T>::line2(const math::vec2<T>& istart, const math::vec2<T>& iend) : start(istart), end(iend)
		{
		}

		///////////////////////////////////////////////////////////////////////////////
		// methods
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline void line2<T>::setLine(const T& xa, const T& ya, const T& xb, const T& yb)
		{
			start.set(xa, ya);
			end.set(xb, yb);
		}

		template <typename T>
		inline void line2<T>::setLine(const math::vec2<T>& nstart, const math::vec2<T>& nend)
		{
			start.set(nstart);
			end.set(nend);
		}

		template <typename T>
		inline void line2<T>::setLine(const line2<T>& line)
		{
			start.set(line.start);
			end.set(line.end);
		}

		template <typename T>
		inline T line2<T>::getLength() const
		{
			return distance(start, end);
		}

		template <typename T>
		inline T line2<T>::getLengthSQ() const
		{
			return distanceSq(start, end);
		}

		template <typename T>
		inline math::vec2<T> line2<T>::getMiddle() const
		{
			return (start + end) * (T)0.5;
		}

		template <typename T>
		inline math::vec2<T> line2<T>::getVector() const
		{
			return end - start;
		}

		template <typename T>
		inline math::vec2<T> line2<T>::getVectorNormalized() const
		{
			math::vec2<double> start64(start.x, start.y);
			math::vec2<double> end64(end.x, end.y);

			math::vec2<double> dir64 = end64 - start64;
			dir64.normalize();

			return math::vec2<T>((T)dir64.x, (T)dir64.y);
		}

		///////////////////////////////////////////////////////////////////////////////
		// intersection
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline bool line2<T>::isPointBetweenStartAndEnd(const math::vec2<T>& point) const
		{
			return isBetweenPoints(point, start, end);
		}

		template <typename T>
		inline math::vec2<T> line2<T>::getClosestPoint(const math::vec2<T>& point) const
		{
			math::vec2<T> c = point - start;
			math::vec2<T> v = end - start;
			T d = (T)v.getLength();
			v /= d;
			T t = math::dot(v, c);

			if (t < (T)0.0)
				return start;
			if (t > d)
				return end;

			v *= t;
			return start + v;
		}

		template <typename T>
		bool line2<T>::getIntersectionWithLine(const line2<T>& other, math::vec2<T>* point, T* outMu)
		{
			//http://www.faqs.org/faqs/graphics/algorithms-faq/
			//this line is from point n0 to point n1
			//the other line is from point m0 to point m1
			const math::vec2<T>& n0 = start;
			const math::vec2<T>& n1 = end;
			const math::vec2<T>& m0 = other.start;
			const math::vec2<T>& m1 = other.end;

			math::vec2<T> n0n1 = n1 - n0;
			math::vec2<T> m1m0 = m0 - m1;
			T D0 = math::cross(n0n1, m1m0);
			if (math::isZero(D0))
			{
				// Segments are parallel
				return false;
			}
			else
			{
				math::vec2<T> n0m0 = m0 - n0;
				T D1 = math::cross(n0m0, m1m0);
				T mu = D1 / D0;
				if (mu >= (T)0 && mu <= (T)1)
				{
					T D2 = math::cross(n0n1, n0m0);
					T rb = D2 / D0;
					if (rb >= (T)0 && rb <= (T)1)
					{
						if (point)
							*point = n0 + n0n1 * mu;
						if (outMu)
							*outMu = mu;
						return true;
					}
				}
				return false;
			}
		}

		template <typename T>
		inline bool line2<T>::getIntersectionWithCircle(math::vec2<T> sorigin, T sradius, double* outdistance) const
		{
			const math::vec2<T> q = sorigin - start;
			T c = q.getLength();
			T v = dot(q, getVector().normalize());
			T d = sradius * sradius - (c*c - v*v);

			if (d < 0.0)
				return false;

			if (outdistance)
				*outdistance = v - math::sqrt((double)d);
			return true;
		}

		template <typename T>
		inline bool line2<T>::getIntersectionWithRect(const rect<T>& rect, math::vec2<T>* point, T* outMu)
		{
			math::vec2<T> ul = rect.upperLeftCorner;
			math::vec2<T> ur = math::vec2<T>(rect.lowerRightCorner.x, rect.upperLeftCorner.y);
			math::vec2<T> ll = math::vec2<T>(rect.upperLeftCorner.x, rect.lowerRightCorner.y);
			math::vec2<T> lr = rect.lowerRightCorner;

			return getIntersectionWithLine(line2<T>(ul, ur), point, outMu)
				|| getIntersectionWithLine(line2<T>(ur, lr), point, outMu)
				|| getIntersectionWithLine(line2<T>(lr, ll), point, outMu)
				|| getIntersectionWithLine(line2<T>(ll, ul), point, outMu);
		}

		///////////////////////////////////////////////////////////////////////////////
		// operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline line2<T> line2<T>::operator+(const math::vec2<T>& point) const
		{
			return line2<T>(start + point, end + point);
		}

		template <typename T>
		inline line2<T> line2<T>::operator-(const math::vec2<T>& point) const
		{
			return line2<T>(start - point, end - point);
		}

		template <typename T>
		inline line2<T>& line2<T>::operator+=(const math::vec2<T>& point)
		{
			start += point;
			end += point;
			return *this;
		}

		template <typename T>
		inline line2<T>& line2<T>::operator-=(const math::vec2<T>& point)
		{
			start -= point;
			end -= point;
			return *this;
		}

		template <typename T>
		inline bool line2<T>::operator==(const line2<T>& other) const
		{
			return (start == other.start && end == other.end) || (end == other.start && start == other.end);
		}

		template <typename T>
		inline bool line2<T>::operator!=(const line2<T>& other) const
		{
			return !(*this == other);
		}

	}
}