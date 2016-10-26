#pragma once

namespace sh
{

	namespace math
	{

		///////////////////////////////////////////////////////////////////////////////
		// forward declarations
		///////////////////////////////////////////////////////////////////////////////
		template<typename T> struct vec3;
		template<typename T> struct vec4;
		template<typename T> struct mat3;

		template <typename T>
		struct mat4
		{
			typedef T value_t;
			static struct ZUninitialized {} uninitialized;


			///////////////////////////////////////////////////////////////////////////////
			// constructors
			///////////////////////////////////////////////////////////////////////////////
			mat4();
			mat4(ZUninitialized);
			explicit mat4(T value);
			mat4(const mat4<T>& other);
			template<typename U>
			explicit mat4(const mat4<U>& v);

			//casting from mat3
			explicit mat4(const mat3<T>& v);
			explicit mat4(const mat2<T>& v);

			//setting from a T array
			explicit mat4(const T values[16]);

			///////////////////////////////////////////////////////////////////////////////
			// methods
			///////////////////////////////////////////////////////////////////////////////

			mat4<T>& set(const T values[16]);

			mat4<T>& setIdentity();
			mat4<T>& setRotationIdentity(); /* DEPRECATED. Use setRotationPartIdentity() */
			mat4<T>& setRotationPartIdentity();
			bool	invert();
			mat4<T>&transpose();

		public:
			//Rows are linearly in memory. First row is m[0], m[1], m[2], m[3] and is the X axis of the matrix
			vec4<T> getRow(uint32_t row) const;
			mat4<T>&setRow(uint32_t row, const vec4<T>& v);

			//Columns are NOT linearly in memory. First row is m[0], m[4], m[8], m[12]
			vec4<T> getColumn(uint32_t column) const;
			mat4<T>&setColumn(uint32_t column, const vec4<T>& v);

			vec3<T> getAxisX() const;
			mat4<T>&setAxisX(const vec3<T>& axis);

			vec3<T> getAxisY() const;
			mat4<T>&setAxisY(const vec3<T>& axis);

			vec3<T> getAxisZ() const;
			mat4<T>&setAxisZ(const vec3<T>& axis);

			const vec3<T>& getTranslation() const;
			mat4<T>&setTranslation(const vec3<T>& trans);

			vec3<T> getScale() const;
			mat4<T>&setScale(const vec3<T>& scale);

			mat3<T> getRotation() const; /* DEPRECATED. Use getRotationPart() */
			mat4<T>&setRotation(const mat3<T>& rot); /* DEPRECATED. Use setRotationPart() */

			mat3<T> getRotationPart() const;
			mat4<T>&setRotationPart(const mat3<T>& rot);

			mat4<T>&postScale(const vec3<T>& scale);
			mat4<T>&postTranslate(const vec3<T>& trans);

			// The method transforms the point using only one column of the matrix
			T dotColumn(uint32_t column, const vec3<T>& point) const;

			///////////////////////////////////////////////////////////////////////////////
			// operators

			bool operator==(const mat4<T>& v) const;
			bool operator!=(const mat4<T>& v) const;

			//casting from mat3
			mat4<T>& operator=(const mat3<T>& m);
			mat4<T>& operator=(const mat2<T>& m);

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
			mat4<T> operator*(T s) const;
			mat4<T>& operator*=(T s);

			///////////////////////////////////////////////////////////////////////////////
			// arithmetic operators
			///////////////////////////////////////////////////////////////////////////////

			mat4<T> operator*(const mat4<T>& other) const;
			mat4<T> operator+(const mat4<T>& other) const;
			mat4<T> operator-(const mat4<T>& other) const;

			//Removed as they were confusing. Use math::transform or math::rotate
			//vec4<T> operator*(const vec4<T>& point) const;
			//Equivalent to mat4 * vec4(v, 1)
			//vec3<T> operator*(const vec3<T>& point) const;

			mat4<T>& operator+=(const mat4<T>& other);
			mat4<T>& operator-=(const mat4<T>& other);

			///////////////////////////////////////////////////////////////////////////////
			//members
			///////////////////////////////////////////////////////////////////////////////
			T m[16];
		};

		///////////////////////////////////////////////////////////////////////////////
		// Implementation
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		typename mat4<T>::ZUninitialized mat4<T>::uninitialized;

		///////////////////////////////////////////////////////////////////////////////

	}//namespace math

}
