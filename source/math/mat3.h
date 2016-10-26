#pragma once

namespace sh
{

	namespace math
	{

		///////////////////////////////////////////////////////////////////////////////
		// forward declarations
		///////////////////////////////////////////////////////////////////////////////
		template<typename T> struct vec2;
		template<typename T> struct vec3;
		template<typename T> struct mat4;

		template <typename T>
		struct mat3
		{
			typedef T value_t;
			static struct ZUninitialized {} uninitialized;

			///////////////////////////////////////////////////////////////////////////////
			// constructors
			///////////////////////////////////////////////////////////////////////////////
			mat3();
			mat3(ZUninitialized);
			explicit mat3(T value);
			mat3(const mat3<T>& other);
			template<typename U>
			explicit mat3(const mat3<U>& v);

			//casting from mat3
			explicit mat3(const mat2<T>& v);
			explicit mat3(const mat4<T>& v);

			//setting from a T array
			explicit mat3(const T values[9]);

			///////////////////////////////////////////////////////////////////////////////
			// methods
			///////////////////////////////////////////////////////////////////////////////

			mat3<T>& set(const T values[9]);

			mat3<T>& setIdentity();
			mat3<T>& setRotationIdentity(); /* DEPRECATED. Use setRotationPartIdentity() */
			mat3<T>& setRotationPartIdentity();
			bool	invert();
			mat3<T>&transpose();

		public:
			//Rows are linearly in memory. First row is m[0], m[1], m[2] and is the X axis of the matrix
			vec3<T> getRow(uint32_t row) const;
			void	setRow(uint32_t row, const vec3<T>& v);

			//Rows are NOT linearly in memory. First row is m[0], m[4], m[8]
			vec3<T> getColumn(uint32_t column) const;
			mat3<T>&setColumn(uint32_t column, const vec3<T>& v);

			vec2<T> getAxisX() const;
			mat3<T>&setAxisX(const vec2<T>& axis);

			vec2<T> getAxisY() const;
			mat3<T>&setAxisY(const vec2<T>& axis);

			const vec2<T>& getTranslation() const;
			mat3<T>&setTranslation(const vec2<T>& trans);

			T		getRotation() const;
			mat3<T>&setRotation(T rot);

			T		getRotationPart() const;
			mat3<T>&setRotationPart(T rot);

			vec2<T> getScale() const;
			mat3<T>&setScale(const vec2<T>& scale);

			mat3<T>& postScale(const vec2<T>& scale);
			mat3<T>& postTranslate(const vec2<T>& trans);

			///////////////////////////////////////////////////////////////////////////////
			// operators

			bool operator==(const mat3<T>& v) const;
			bool operator!=(const mat3<T>& v) const;

			//casting from mat3
			mat3<T>& operator=(const mat2<T>& m);
			mat3<T>& operator=(const mat4<T>& m);

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
			mat3<T> operator*(T scalar) const;
			mat3<T>& operator*=(T scalar);

			///////////////////////////////////////////////////////////////////////////////
			// arithmetic operators
			///////////////////////////////////////////////////////////////////////////////

			mat3<T> operator*(const mat3<T>& other) const;
			mat3<T> operator+(const mat3<T>& other) const;
			mat3<T> operator-(const mat3<T>& other) const;

			//Removed as they were confusing. Use math::transform or math::rotate
			//vec3<T> operator*(const vec3<T>& point) const;
			//Equivalent to mat3 * vec3(v, 1)
			//vec2<T> operator*(const vec2<T>& point) const;

			mat3<T>& operator+=(const mat3<T>& other);
			mat3<T>& operator-=(const mat3<T>& other);

			///////////////////////////////////////////////////////////////////////////////
			//members
			///////////////////////////////////////////////////////////////////////////////
			T m[9];
		};

		///////////////////////////////////////////////////////////////////////////////
		// Implementation
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		typename mat3<T>::ZUninitialized mat3<T>::uninitialized;

		///////////////////////////////////////////////////////////////////////////////

	}//namespace math

}
