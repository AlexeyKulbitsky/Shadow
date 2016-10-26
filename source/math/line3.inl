namespace sh
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////
		// constructors
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline line3<T>::line3() : start(0, 0, 0), end(1, 1, 1)
		{
		}

		template <typename T>
		inline line3<T>::line3(T xa, T ya, T za, T xb, T yb, T zb) : start(xa, ya, za), end(xb, yb, zb)
		{
		}

		template <typename T>
		inline line3<T>::line3(const vec3<T>& istart, const vec3<T>& iend) : start(istart), end(iend)
		{
		}

		///////////////////////////////////////////////////////////////////////////////
		// methods
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline void line3<T>::setLine(const T& xa, const T& ya, const T& za, const T& xb, const T& yb, const T& zb)
		{
			start.set(xa, ya, za);
			end.set(xb, yb, zb);
		}

		template <typename T>
		inline void line3<T>::setLine(const vec3<T>& nstart, const vec3<T>& nend)
		{
			start = nstart;
			end = nend;
		}

		template <typename T>
		inline void line3<T>::setLine(const line3<T>& line)
		{
			start.set(line.start);
			end.set(line.end);
		}

		template <typename T>
		inline T line3<T>::getLength() const
		{
			return distance(start, end);
		}

		template <typename T>
		inline T line3<T>::getLengthSQ() const
		{
			return distanceSq(start, end);
		}

		template <typename T>
		inline vec3<T> line3<T>::getMiddle() const
		{
			return (start + end) * (T)0.5;
		}

		template <typename T>
		inline vec3<T> line3<T>::getVector() const
		{
			return end - start;
		}

		template <typename T>
		inline vec3<T> line3<T>::getVectorNormalized() const
		{
			vec3<double> s(start);
			vec3<double> e(end);
			vec3<double> dir = math::normalized(e - s);
			return vec3<T>(dir);
		}

		template <typename T>
		inline bool line3<T>::isBetweenPoints(const vec3<T>& point, const vec3<T>& begin, const vec3<T>& end) const
		{
			const T f = (end - begin).getLengthSq();
			return distanceSq(point, begin) <= f && distanceSq(point, end) <= f;
		}


		template <typename T>
		inline bool line3<T>::isPointBetweenStartAndEnd(const vec3<T>& point) const
		{
			return isBetweenPoints(point, start, end);
		}

		template <typename T>
		inline vec3<T> line3<T>::getClosestPoint(const vec3<T>& point) const
		{
			vec3<T> c = point - start;
			vec3<T> v = end - start;
			T d = (T)v.getLength();
			if (isZero(d))// if line is a point, then the closest point is the point itself
			{
				return start;
			}
			v /= d;
			T t = dot(v, c);

			if (t < (T)0.0)
				return start;
			if (t > d)
				return end;

			v *= t;
			return start + v;
		}

		template <typename T>
		inline T line3<T>::getClosestPointProjected(const vec3<T>& point) const
		{
			vec3<T> c = point - start;
			vec3<T> v = end - start;
			T d = (T)v.getLength();
			v /= d;
			T t = dot(v, c);

			if (t < (T)0)
				return (T)0;
			if (t > d)
				return (T)1;
			else
				return t / d;
		}

		template <typename T>
		inline bool line3<T>::isIntersectingWithSphere(const vec3<T>& sphereCenter, T sphereRadius) const
		{
			const math::vec3<T> q = sphereCenter - start;
			T c = q.getLengthSq();
			T v = math::dot(q, getVectorNormalized());
			T d = sphereRadius * sphereRadius - (c - v*v);
			return d > 0;
		}

		template <typename T>
		inline bool line3<T>::getIntersectionWithSphere(const vec3<T>& sphereCenter, T sphereRadius, double& outdistance) const
		{
			const math::vec3<T> q = sphereCenter - start;
			T c = q.getLengthSq();
			T v = math::dot(q, getVectorNormalized());
			T d = sphereRadius * sphereRadius - (c - v*v);

			if (d < 0.0)
				return false;

			outdistance = v - math::sqrt((double)d);
			return true;
		}

		template <typename T>
		inline bool line3<T>::getIntersectionWithSphere(const vec3<T>& sphereCenter, T sphereRadius, double& outdistance1, double& outdistance2) const
		{
			// http://www.devmaster.net/wiki/Ray-sphere_intersection
			const vec3<T> D = getVectorNormalized();

			vec3<T> v = start - sphereCenter;
			T b = T(2) * dot(v, D);
			T c = -sphereRadius*sphereRadius + dot(v, v);
			T d = b * b - T(4) * c;

			if (d >= (T)0)
			{
				d = math::sqrt(d);
				outdistance1 = (-b - d) * T(0.5);
				outdistance2 = (-b + d) * T(0.5);
				return true;
			}

			return false;
		}

		template <typename T>
		inline bool line3<T>::isPointOnLine(const vec3<T>& point) const
		{
			/*the point is on the line if there exists some t such that
			start + getVector()*t==point =>
			getVector()*t==point-start =>
			getVector().normalize()==(point-start).normalize()
			we also need to account for a negative T, however
			*/
			vec3<T> vec1(getVector().normalize());
			vec3<T> vec2((point - start).normalize());

			bool onInfiniteLine = equals(vec1, vec2) || equals(vec1, -vec2);
			return onInfiniteLine;
		}

		//returns true if there is an intersection and fills outPoint
		//algorithm taken from http://www.mc.edu/campus/users/travis/maa/proceedings/spring2001/bard.himel.pdf
		template <typename T>
		inline bool line3<T>::getIntersectionWithLine(const line3<T>& other, vec3<T>& intersection) const
		{
			//handle the case of being parallel separately
			vec3<T> dir = getVector().normalize();
			vec3<T> otherDir = other.getVector().normalize();

			T dotf = dot(dir, otherDir);

			if (equals(abs(dotf - 1.0f), 0.0f))
			{
				//std::cout<<"getIntersection(): is parallel\n";
				//the lines are parallel, special case
				if (!isPointOnLine(other.start))
				{
					//std::cout<<"doesn't lie on infinite line\n";
					return false;
				}
				//there are multiple intersection points
				//we don't try to figure out which one the user wants,
				//and just give him the first one we find
				if (isPointBetweenStartAndEnd(other.start))
				{
					intersection = other.start;
					return true;
				}
				if (isPointBetweenStartAndEnd(other.end))
				{
					intersection = other.end;
					return true;
				}
				if (other.isPointBetweenStartAndEnd(start))
				{
					intersection = start;
					return true;
				}
				//std::cout<<"none were between start and end\n";
				return false;
			}
			else
			{
				T A = getVector().getLengthSq();

				T B = 2 * (dot(getVector(), start) - dot(getVector(), other.start));
				T C = 2 * (dot(getVector(), other.getVector()));
				T D = 2 * (dot(other.getVector(), other.start) - dot(other.getVector(), start));
				T E = other.getVector().getLengthSq();
				T s = (2 * A*D + B*C) / (C*C - 4 * A*E);
				T t = (C*s - B) / (2 * A);
				vec3<T> point1 = start + getVector() * t;
				vec3<T> point2 = other.start + other.getVector() * s;
				//std::cout<<"points are "<<point1<<",   "<<point2<<"\n";
				if (equals(point1, point2))
				{
					if (isPointBetweenStartAndEnd(point1) && other.isPointBetweenStartAndEnd(point1))
					{
						intersection = point1;
						return true;
					}
					return false;

				}
				else
				{
					return false;
				}
			}
		}
		///////////////////////////////////////////////////////////////////////////////
		// operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline line3<T> line3<T>::operator+(const vec3<T>& point) const
		{
			return line3<T>(start + point, end + point);
		}

		template <typename T>
		inline line3<T>& line3<T>::operator+=(const vec3<T>& point)
		{
			start += point;
			end += point;
			return *this;
		}

		template <typename T>
		inline line3<T> line3<T>::operator-(const vec3<T>& point) const
		{
			return line3<T>(start - point, end - point);
		}

		template <typename T>
		inline line3<T>& line3<T>::operator-=(const vec3<T>& point)
		{
			start -= point;
			end -= point;
			return *this;
		}

		template <typename T>
		inline bool line3<T>::operator==(const line3<T>& other) const
		{
			return (start == other.start && end == other.end) || (end == other.start && start == other.end);
		}

		template <typename T>
		inline bool line3<T>::operator!=(const line3<T>& other) const
		{
			return !(*this == other);
		}

	}
}