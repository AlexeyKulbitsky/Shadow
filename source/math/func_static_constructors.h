#pragma once

///////////////////////////////////////////////////////////////////////////////
// Defines transform functions
///////////////////////////////////////////////////////////////////////////////
namespace sh
{


	namespace math
	{
		///! Creates a world space lookat matrix (front axis is -yaxis, up axis is zaxis)
		template<typename T>
		mat3<T> lookAt(const vec3<T>& front, const vec3<T>& up);

		template<typename T>
		quat<T> quatLookAt(const vec3<T>& front, const vec3<T>& up);

		template<typename T>
		quat<T> quatAxisX(T rot);

		template<typename T>
		quat<T> quatAxisY(T rot);

		template<typename T>
		quat<T> quatAxisZ(T rot);


		// LEGACY - wrong order - TO REMOVE
		template <typename T>
		quat<T> quatZXY(T rotZ, T rotX, T rotY);

		template <typename T>
		void quatZXY(const quat<T>& q, T& rotZ, T& rotX, T& rotY);

		template <typename T>
		quat<T> quatRotationFromTo(const vec3<T>& a, const vec3<T>& b);

	}//namespace math

}
