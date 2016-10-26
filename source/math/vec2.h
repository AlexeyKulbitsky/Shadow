#pragma once

namespace sh
{

	namespace math
	{
		template<typename T> struct vec3;
		template<typename T> struct vec4;

		template<typename T>
		struct vec2
		{
			typedef T value_t;
			T x, y;

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// constructors
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

			static struct ZUninitialized {} uninitialized;

			//////////////////////////////////////
			// Basic constructors
			vec2();

			//constructs the vector uninitialized
			vec2(ZUninitialized);

			//constructs from direct x, y values
			vec2(T x, T y);

			//construct from a single value - x == y == s
			//this has to be explicit to avoid undesirable casts
			explicit vec2(T s);

			//copy constructor
			vec2(const vec2<T>& v);

			// Conversion vector constructors
			template<typename U>
			explicit vec2(const vec2<U>& v);

			//casting
			//implementation is in cast.inl
			explicit vec2(const vec3<T>& v);
			explicit vec2(const vec4<T>& v);

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// member functions
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

			vec2<T>& set(T x, T y);
			T getLength() const;
			T getLengthSq() const;
			void setLength(T len);
			vec2<T>& normalize();

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// operators
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

			T& operator[](uint32_t i);
			const T& operator[](uint32_t i) const;

			bool operator==(const vec2<T>& v) const;
			bool operator!=(const vec2<T>& v) const;

			vec2<T> operator-() const;

			vec2<T> operator+(T s) const;
			vec2<T> operator+(const vec2<T>& v2) const;

			vec2<T> operator-(T s) const;
			vec2<T> operator-(const vec2<T>& v2) const;

			vec2<T> operator*(T s) const;
			vec2<T> operator/(T s) const;
			vec2<T>& operator=(const vec2<T>& v);

			//casting
			//implementation is in cast.inl
			vec2<T>& operator=(const vec3<T>& v);
			vec2<T>& operator=(const vec4<T>& v);

			vec2<T>& operator+=(T s);
			vec2<T>& operator+=(const vec2<T>& v);

			vec2<T>& operator-=(T s);
			vec2<T>& operator-=(const vec2<T>& v);

			vec2<T>& operator*=(T s);
			vec2<T>& operator*=(const vec2<T>& v);

			vec2<T>& operator/=(T s);
			vec2<T>& operator/=(const vec2<T>& v);
		};

		template <typename T>
		vec2<T> operator*(T s, const vec2<T>&);
		template <typename T>
		vec2<T> operator/(T s, const vec2<T>&);

		template<typename T>
		vec2<T> operator*(const vec2<T>& v0, const vec2<T>& v1);

		template<typename T>
		vec2<T> operator/(const vec2<T>& v0, const vec2<T>& v2);

		//////////////////////////////////////////////////////////////////////////

		template <typename T>
		typename vec2<T>::ZUninitialized vec2<T>::uninitialized;

	} //math

}