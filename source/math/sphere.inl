namespace sh
{
	namespace math
	{


		template <class T>
		sphere<T>::sphere(const math::vec3<T>& _center, T _radius)
			: center(_center)
			, radius(_radius)
		{

		}

		template <class T>
		inline void sphere<T>::setCenter(const math::vec3<T>& _center)
		{
			center = _center;
		}

		template <class T>
		inline void sphere<T>::setRadius(T _radius)
		{
			radius = _radius;
		}

		template <class T>
		inline const math::vec3<T>& sphere<T>::getCenter() const
		{
			return center;
		}

		template <class T>
		inline T sphere<T>::getRadius() const
		{
			return radius;
		}

		template <class T>
		inline bool sphere<T>::contains(const math::vec3<T>& point) const
		{
			const T distanceSqr = (center - point).getLengthSq();
			return (distanceSqr <= radius * radius);
		}

		template <class T>
		inline bool sphere<T>::contains(const sphere<T>& other) const
		{
			// 	const T distanceBetweenCenters = (center - other.center).getLength();
			// 	return (distanceBetweenCenters + other.radius <= radius);
			const T distanceSq = (center - other.center).getLengthSq();
			const T radiusDiffSq = math::square(radius - other.radius);
			return (distanceSq <= radiusDiffSq);
		}

		template <class T>
		inline bool sphere<T>::overlaps(const sphere<T>& other) const
		{
			const T totalRadius = radius + other.radius;
			const T distanceSqr = (center - other.center).getLengthSq();
			return (distanceSqr <= totalRadius * totalRadius);
		}

		template <class T>
		inline T sphere<T>::distance(const math::vec3<T>& point) const
		{
			const T distance = (center - point).getLength();
			return (distance - radius);
		}


		//! Perform a union between two spheres. The result will contain this sphere and the incoming one.
		template <class T>
		void sphere<T>::meld(const sphere<T>& other)
		{
			if (radius == (T)0)
			{
				if (other.radius != (T)0)
					*this = other;
			}
			else
			{
				math::vec3<T> vec = other.center - center;
				T module = vec.getLength();

				// Check that the other sphere is not contained by this one. Otherwise leave.
				if (module + other.radius > radius)
				{
					if (module + radius <= other.radius)
					{
						// This sphere is contained by the other one.
						*this = other;
					}
					else
					{
						T centerLambda = (module + other.radius + radius) * (T)(0.5);
						center = center + vec / module * (centerLambda - radius);
						radius = centerLambda;
					}
				}
			}
		}

		//! Perform a union between this sphere and a point. The result will contain this sphere and the incoming point.
		template <class T>
		void sphere<T>::meld(const math::vec3<T>& point)
		{
			// If this sphere is empty then do anything.
			if (radius == (T)0)
			{
				center = point;
				radius = std::numeric_limits<T>::min(); // make it slightly different from zero so it is not considered as empty
			}
			else if (!contains(point))
			{
				math::vec3<T> vec = point - center;
				vec.normalize();

				math::vec3<T> p = center - vec * radius;

				center = (p + point) * (T)(0.5);
				radius = (p - point).getLength() * (T)(0.5);
			}
		}

		template <class T>
		inline bool sphere<T>::operator == (const sphere<T>& other) const
		{
			return (center == other.center  &&  math::equals(radius, other.radius));
		}

		template <class T>
		inline bool sphere<T>::operator != (const sphere<T>& other) const
		{
			return !((*this) == other);
		}

	}
}