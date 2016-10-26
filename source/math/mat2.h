#pragma once

namespace sh
{

	namespace math
	{

		///////////////////////////////////////////////////////////////////////////////
		// forward declarations
		///////////////////////////////////////////////////////////////////////////////
		template<typename T> struct vec2;
		template<typename T> struct mat3;
		template<typename T> struct mat4;

		template <typename T>
		struct mat2
		{
			typedef T value_t;
			static struct ZUninitialized {} uninitialized;

			///////////////////////////////////////////////////////////////////////////////
			// constructors
			///////////////////////////////////////////////////////////////////////////////
			mat2();
			mat2(ZUninitialized);
			explicit mat2(T value);
			mat2(const mat2<T>& other);
			template<typename U>
			explicit mat2(const mat2<U>& v);

			//casting
			explicit mat2(const mat3<T>& v);
			explicit mat2(const mat4<T>& v);


			//setting from a T array
			explicit mat2(const T values[4]);

			///////////////////////////////////////////////////////////////////////////////
			// methods
			///////////////////////////////////////////////////////////////////////////////

			mat2<T>& set(const T values[4]);

			mat2<T>& setIdentity();
			bool	invert();
			mat2<T>&transpose();

		public:
			//Rows are linearly in memory. First row is m[0], m[1], m[2] and is the X axis of the matrix
			vec2<T> getRow(uint32_t row) const;
			mat2<T>&setRow(uint32_t row, const vec2<T>& v);

			//Rows are NOT linearly in memory. First row is m[0], m[4], m[8]
			vec2<T> getColumn(uint32_t column) const;
			mat2<T>&setColumn(uint32_t column, const vec2<T>& v);

			vec2<T> getAxisX() const;
			mat2<T>&setAxisX(const vec2<T>& axis);

			vec2<T> getAxisY() const;
			mat2<T>&setAxisY(const vec2<T>& axis);

			vec2<T> getScale() const;
			mat2<T>&setScale(const vec2<T>& scale);

			///////////////////////////////////////////////////////////////////////////////
			// operators

			bool operator==(const mat2<T>& v) const;
			bool operator!=(const mat2<T>& v) const;

			//casting
			mat2<T>& operator=(const mat4<T>& m);
			mat2<T>& operator=(const mat3<T>& m);

			///////////////////////////////////////////////////////////////////////////////
			// indexing operators
			///////////////////////////////////////////////////////////////////////////////
			operator T*();
			operator const T*() const;

			T& operator()(uint32_t row, uint32_t col);
			const T& operator()(uint32_t row, uint32_t col) const;

			///////////////////////////////////////////////////////////////////////////////
			// scalar operators
			///////////////////////////////////////////////////////////////////////////////
			mat2<T> operator*(T scalar) const;
			mat2<T>& operator*=(T scalar);

			///////////////////////////////////////////////////////////////////////////////
			// arithmetic operators
			///////////////////////////////////////////////////////////////////////////////

			mat2<T> operator*(const mat2<T>& other) const;
			mat2<T> operator+(const mat2<T>& other) const;
			mat2<T> operator-(const mat2<T>& other) const;

			//Removed as they were confusing. Use math::transform or math::rotate
			//vec2<T> operator*(const vec2<T>& point) const;

			mat2<T>& operator+=(const mat2<T>& other);
			mat2<T>& operator-=(const mat2<T>& other);

			///////////////////////////////////////////////////////////////////////////////
			//members
			///////////////////////////////////////////////////////////////////////////////
			T m[4];
		};

		///////////////////////////////////////////////////////////////////////////////
		// Implementation
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		typename mat2<T>::ZUninitialized mat2<T>::uninitialized;

		///////////////////////////////////////////////////////////////////////////////

	}//namespace math

}
