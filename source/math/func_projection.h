#pragma once
namespace sh
{


	namespace math
	{
		///! Creates a RH centered orthographic projection matrix
		template<typename T>
		mat4<T> orthoCenter(T width, T height, T zNear, T zFar);

		///! Creates a RH offseted orthographic projection matrix
		template<typename T>
		mat4<T> ortho(T left, T right, T bottom, T top, T zNear, T zFar);

		///! Creates a RH perspective projection matrix
		template<typename T>
		mat4<T> perspective(T fovRadians, T aspectRatio, T zNear, T zFar);

		///! Creates a RH asymmetric frustum projection matrix
		template<typename T>
		mat4<T> perspective(T left, T right, T bottom, T top, T zNear, T zFar);

	}//namespace math

}
