#pragma once

namespace sh
{

	namespace math
	{

		template <typename T>
		struct quat
		{
			typedef T value_t;
			T x, y, z, w;

			static struct ZUninitialized {} uninitialized;
			static struct ZAxisX {} axisX;
			static struct ZAxisY {} axisY;
			static struct ZAxisZ {} axisZ;
			static struct ZAngleAxis {} angleAxis;
			static struct ZEulerXYZ {} eulerXYZ;
			static struct ZEulerXZY {} eulerXZY;
			static struct ZEulerYXZ {} eulerYXZ;
			static struct ZEulerYZX {} eulerYZX;
			static struct ZEulerZXY {} eulerZXY;
			static struct ZEulerZYX {} eulerZYX;

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// constructors
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

			quat();
			quat(ZUninitialized);
			quat(T x, T y, T z, T w);
			quat(const quat&);

			//conversion constructors
			quat(const mat3<T>&);
			quat(const mat4<T>&);

			//these use the enum to avoid ambiguities
			//use as quatf(quatf::axisX, pi)
			//or quatf(quatf::angleAxis, pi, vec3f(0, 0, 1))
			quat(ZAxisX, T angle);
			quat(ZAxisY, T angle);
			quat(ZAxisZ, T angle);
			quat(ZAngleAxis, T angle, const vec3<T>& axis);

			//These handle conversion from Euler (x, y, z) to quat
			//All possible orders of multiplication are handled.
			//NOTE - this doesn't imply any yaw/pitch/roll - the user gives that meaning.
			quat(ZEulerXYZ, const vec3<T>& angles);
			quat(ZEulerXZY, const vec3<T>& angles);
			quat(ZEulerYXZ, const vec3<T>& angles);
			quat(ZEulerYZX, const vec3<T>& angles);
			quat(ZEulerZXY, const vec3<T>& angles);
			quat(ZEulerZYX, const vec3<T>& angles);

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// member functions
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//! Direct assignment
			quat<T>& set(T x, T y, T z, T w);

			//! Normalizes the quat
			quat<T>& normalize();

			//! Returns the length
			T		 getLength() const;

			//! Returns the length
			T		 getLengthSq() const;

			//! Inverts this quat
			quat<T>& invert();

			//! Set quat to identity
			quat<T>& setIdentity();

			//! cast from/to mat3
			quat<T>& setFromMat3(const mat3<T>& mat);
			mat3<T>  getAsMat3() const;

			//! cast from/to mat4
			quat<T>& setFromMat4(const mat4<T>& mat);
			mat4<T>  getAsMat4() const;

			//! cast from/to angle axis
			quat<T>& setFromAngleAxis(T angle, const vec3<T>& axis);
			void	 getAsAngleAxis(T& angle, vec3<T>& axis) const;

			//! cast from/to euler (polar rotations in a vector)
			//these handle conversion from Euler (x, y, z) to quat
			//All possible orders of multiplication are handled.
			//NOTE - this doesn't imply any yaw/pitch/roll - the user gives that meaning.
			quat<T>& setFromEulerXYZ(const vec3<T>& angles);
			vec3<T>  getAsEulerXYZ() const;
			quat<T>& setFromEulerXZY(const vec3<T>& angles);
			vec3<T>  getAsEulerXZY() const;

			quat<T>& setFromEulerYXZ(const vec3<T>& angles);
			vec3<T>  getAsEulerYXZ() const;
			quat<T>& setFromEulerYZX(const vec3<T>& angles);
			vec3<T>  getAsEulerYZX() const;

			quat<T>& setFromEulerZXY(const vec3<T>& angles);
			vec3<T>  getAsEulerZXY() const;
			quat<T>& setFromEulerZYX(const vec3<T>& angles);
			vec3<T>  getAsEulerZYX() const;

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// operators
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

			bool operator==(const quat<T>& v) const;
			bool operator!=(const quat<T>& v) const;

			//! Assignment operator
			quat& operator=(const quat& other);

			//! Add operator
			quat operator+(const quat& other) const;

			//! Subtract operator
			quat operator-(const quat& other) const;

			//! Multiplication operator
			quat operator*(const quat& other) const;

			//! Multiplication operator
			quat& operator*=(const quat& other);

			//! Multiplication operator with scalar
			//template <typename U>
			quat<T> operator*(T s) const;

			// template <typename U>
			 //vec3<T> operator*(const vec3<U>& s) const;

			 //! Multiplication operator with scalar
			 //template <typename U> 
			quat& operator*=(T s);

			//! Negation (additive inverse) (-x,-y,-z,-w).
			// A + (-A) == A - A == identity
			quat operator-() const;

			//! Conjugation (spatial inverse) (-x,-y,-z,+w).
			// A * (~A) == identity
			quat operator~() const;
		};//quaternion

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// global functions
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		template <typename T>
		T dot(const quat<T>& q1, const quat<T>& q2);

		template <typename T, typename U>
		vec3<U> operator*(const quat<T>& q, const vec3<U>& v);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// implementation
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		template <typename T>
		typename quat<T>::ZUninitialized quat<T>::uninitialized;
		template <typename T>
		typename quat<T>::ZAxisX quat<T>::axisX;
		template <typename T>
		typename quat<T>::ZAxisY quat<T>::axisY;
		template <typename T>
		typename quat<T>::ZAxisZ quat<T>::axisZ;
		template <typename T>
		typename quat<T>::ZAngleAxis quat<T>::angleAxis;
		template <typename T>
		typename quat<T>::ZEulerXYZ quat<T>::eulerXYZ;
		template <typename T>
		typename quat<T>::ZEulerXZY quat<T>::eulerXZY;
		template <typename T>
		typename quat<T>::ZEulerYXZ quat<T>::eulerYXZ;
		template <typename T>
		typename quat<T>::ZEulerYZX quat<T>::eulerYZX;
		template <typename T>
		typename quat<T>::ZEulerZXY quat<T>::eulerZXY;
		template <typename T>
		typename quat<T>::ZEulerZYX quat<T>::eulerZYX;

	}//math

}
