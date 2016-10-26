#pragma once

///////////////////////////////////////////////////////////////////////////////
// Defines transform functions
///////////////////////////////////////////////////////////////////////////////
namespace sh
{

	namespace math
	{
		//////////////////////////////////////////////////////////////////////////

		//equivalent to m = m1 * m2
		template<typename T>
		mat3<T> multiply(const mat3<T>& m1, const mat3<T>& m2);

		//multiplies only the 3x2 parts of the matrices
		template<typename T>
		mat3<T> multiply32(const mat3<T>& m1, const mat3<T>& m2);

		//equivalent to m = m1 * m2
		template<typename T>
		mat4<T> multiply(const mat4<T>& m1, const mat4<T>& m2);

		//multiplies only the 4x3 parts of the matrices
		template<typename T>
		mat4<T> multiply43(const mat4<T>& m1, const mat4<T>& m2);

		//////////////////////////////////////////////////////////////////////////
		// transforms
		//////////////////////////////////////////////////////////////////////////

		//Multiplies all vector components with the matrix.
		template<typename T>
		vec2<T> transform(const mat2<T>& m, const vec2<T>& v);

		//Multiplies all vector components with the matrix.
		template<typename T>
		vec3<T> transform(const mat3<T>& m, const vec3<T>& v);

		//Multiplies all vector components with the matrix.
		template<typename T>
		vec4<T> transform(const mat4<T>& m, const vec4<T>& v);

		//Equivalent to transform(mat4, vec4(v, 1))
		template<typename T>
		vec3<T> transform(const mat4<T>& m, const vec3<T>& v);

		//Equivalent to transform(mat3, vec3(v, 1))
		template<typename T>
		vec2<T> transform(const mat3<T>& m, const vec2<T>& v);

		//////////////////////////////////////////////////////////////////////////
		// Rotations
		//////////////////////////////////////////////////////////////////////////

		//Equivalent to transform(mat3, vec3(v, 0))
		//Transforms but without applying translation
		template<typename T>
		vec2<T> rotate(const mat3<T>& m, const vec2<T>& v);

		//Equivalent to transform(mat4, vec4(v, 0))
		//Transforms but without applying translation
		template<typename T>
		vec3<T> rotate(const mat4<T>& m, const vec3<T>& v);

		template<typename T>
		vec3<T> rotate(const quat<T>& q, const vec3<T>& v);

		//////////////////////////////////////////////////////////////////////////
		// Project
		//////////////////////////////////////////////////////////////////////////

		//Transforms the vector and divides by the W
		//The W is returned undivided.
		template<typename T>
		vec4<T> project(const mat4<T>& m, const vec4<T>& v);

		//Equivalent to project(mat4, vec4(v, 1))
		template<typename T>
		vec3<T> project(const mat4<T>& m, const vec3<T>& v);

		//////////////////////////////////////////////////////////////////////////
		//

		template<typename T>
		void transformArray(const mat4<T>& m, uint32_t count, const vec3<T>* input, vec3<T>* output);
		template<typename T>
		void transformArray(const mat4<T>& m, uint32_t count, const vec4<T>* input, vec4<T>* output);


	}//namespace math

}
