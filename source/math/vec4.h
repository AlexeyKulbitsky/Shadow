#pragma once

namespace sh
{

	namespace math
	{
		template<typename T> struct vec2;
		template<typename T> struct vec3;


		template<typename T>
		struct vec4
		{
			typedef T value_t;
			T x, y, z, w;

			static struct ZUninitialized {} uninitialized;

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// constructors
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//////////////////////////////////////
				// Basic constructors
			vec4();

			//constructs the vector uninitialized
			vec4(ZUninitialized);

			//constructs from direct x, y, z values
			vec4(T x, T y, T z, T w);

			//construct from a single value - x == y == z = s
			//this has to be explicit to avoid undesirable casts
			explicit vec4(T s);

			//copy constructor
			vec4(const vec4<T>& v);

			// Conversion vector constructors
			template<typename U>
			explicit vec4(const vec4<U>& v);

			//cast
			//implementation in cast.inl
			explicit vec4(const vec2<T>& v);
			explicit vec4(const vec2<T>& v, T z, T w);
			explicit vec4(const vec3<T>& v);
			explicit vec4(const vec3<T>& v, T w);

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// member functions
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////////
				// Methods
				// Use only non-mutable methods here pls

			vec4<T>& set(T x, T y, T z, T w);
			T getLength() const;
			T getLengthSq() const;
			void setLength(T len);
			vec4<T>& normalize();

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// operators
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

			T& operator[](uint32_t i);
			const T& operator[](uint32_t i) const;

			bool operator==(const vec4<T>& v) const;
			bool operator!=(const vec4<T>& v) const;

			vec4<T> operator-() const;

			vec4<T> operator+(T s) const;
			vec4<T> operator+(const vec4<T>& v) const;

			vec4<T> operator-(T s) const;
			vec4<T> operator-(const vec4<T>& v) const;

			vec4<T> operator*(T s) const;
			vec4<T> operator/(T s) const;

			vec4<T>& operator=(const vec4<T>& v);
			vec4<T>& operator=(const vec3<T>& v);
			vec4<T>& operator=(const vec2<T>& v);

			vec4<T>& operator+=(T s);
			vec4<T>& operator+=(const vec4<T>& v);

			vec4<T>& operator-=(T s);
			vec4<T>& operator-=(const vec4<T>& v);

			vec4<T>& operator*=(T s);
			vec4<T>& operator*=(const vec4<T>& v);

			vec4<T>& operator/=(T s);
			vec4<T>& operator/=(const vec4<T>& v);
		};

		template <typename T>
		math::vec4<T> operator*(T s, const math::vec4<T>&);
		template <typename T>
		math::vec4<T> operator/(T s, const math::vec4<T>&);


		template <typename T>
		math::vec4<T> operator*(const math::vec4<T>&, const math::vec4<T>&);

		template <typename T>
		math::vec4<T> operator/(const math::vec4<T>&, const math::vec4<T>&);


		//////////////////////////////////////////////////////////////////////////

		template <typename T>
		typename vec4<T>::ZUninitialized vec4<T>::uninitialized;

	} //math

}
