#pragma once

namespace sh
{

	namespace math
	{
		template<typename T> struct vec2;
		template<typename T> struct vec4;

		template<typename T>
		struct vec3
		{
			typedef T value_t;
			T x, y, z;

			static struct ZUninitialized {} uninitialized;

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// constructors
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//////////////////////////////////////
				// Basic constructors
			vec3();

			//constructs the vector uninitialized
			vec3(ZUninitialized);

			//constructs from direct x, y, z values
			vec3(T x, T y, T z);

			//construct from a single value - x == y == z = s
			//this has to be explicit to avoid undesirable casts
			explicit vec3(T s);

			//copy constructor
			vec3(const vec3<T>& v);

			// Conversion vector constructors
			template<typename U>
			explicit vec3(const vec3<U>& v);

			//cast
			//implementation is in cast.inl
			explicit vec3(const vec2<T>& v);
			explicit vec3(const vec2<T>& v, T z);
			explicit vec3(const vec4<T>& v);

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// member functions
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////////
				// Methods
				// Use only non-mutable methods here pls

			vec3<T>& set(T x, T y, T z);
			T getLength() const;
			T getLengthSq() const;
			void setLength(T len);
			vec3<T>& normalize();

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// operators
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

			T& operator[](uint32_t i);
			const T& operator[](uint32_t i) const;

			bool operator==(const vec3<T>& v) const;
			bool operator!=(const vec3<T>& v) const;

			vec3<T> operator-() const;

			vec3<T> operator+(T s) const;
			vec3<T> operator+(const vec3<T>& v) const;

			vec3<T> operator-(T s) const;
			vec3<T> operator-(const vec3<T>& v) const;

			vec3<T> operator*(T s) const;
			vec3<T> operator/(T s) const;

			vec3<T>& operator=(const vec3<T>& v);

			//cast
			//implementation is in cast.inl
			vec3<T>& operator=(const vec2<T>& v);
			vec3<T>& operator=(const vec4<T>& v);

			vec3<T>& operator+=(T s);
			vec3<T>& operator+=(const vec3<T>& v);

			vec3<T>& operator-=(T s);
			vec3<T>& operator-=(const vec3<T>& v);

			vec3<T>& operator*=(T s);
			vec3<T>& operator*=(const vec3<T>& v);

			vec3<T>& operator/=(T s);
			vec3<T>& operator/=(const vec3<T>& v);
		};

		template <typename T>
		vec3<T> operator*(T s, const vec3<T>&);

		template<typename T>
		vec3<T> operator*(const vec3<T>& v0, const vec3<T>& v1);

		template <typename T>
		vec3<T> operator/(T s, const vec3<T>&);

		template <typename T>
		vec3<T> operator/(const vec3<T>& v0, const vec3<T>& v1);

		//////////////////////////////////////////////////////////////////////////

		template <typename T>
		typename vec3<T>::ZUninitialized vec3<T>::uninitialized;

	} //math

}
