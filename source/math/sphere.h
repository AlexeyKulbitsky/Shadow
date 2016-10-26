#pragma once
namespace sh
{


	namespace math
	{

		template <class T>
		struct sphere
		{

			///////////////////////////////////////////////////////////////////////////////
			// constructors
			///////////////////////////////////////////////////////////////////////////////

			explicit sphere(const math::vec3<T>& _center = math::vec3<T>((T)0, (T)0, (T)0), T _radius = (T)0);

			///////////////////////////////////////////////////////////////////////////////
			// operators
			///////////////////////////////////////////////////////////////////////////////

			bool operator == (const sphere<T>& other) const;
			bool operator != (const sphere<T>& other) const;

			///////////////////////////////////////////////////////////////////////////////
			// methods
			///////////////////////////////////////////////////////////////////////////////

			void setCenter(const math::vec3<T>& _center);
			void setRadius(T _radius);

			const math::vec3<T>& getCenter() const;
			T getRadius() const;

			bool contains(const math::vec3<T>& point) const;

			bool contains(const sphere<T>& other) const;

			bool overlaps(const sphere<T>& other) const;

			// negative distance means inside the sphere
			T distance(const math::vec3<T>& point) const;

			//! Perform a union between two spheres. The result will contain this sphere and the incoming one.
			void meld(const sphere<T>& other);

			//! Perform a union between this sphere and a point. The result will contain this sphere and the incoming point.
			void meld(const math::vec3<T>& point);

			math::vec3<T>	center;
			T				radius;
		};


		typedef sphere<float>  spheref;


	}

}
