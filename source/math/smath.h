#ifndef SHADOW_MATH_INCLUDE
#define SHADOW_MATH_INCLUDE

#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"

namespace sh
{
	namespace math
	{
		typedef Vector2<short> Vector2s;
		typedef Vector2<int> Vector2i;
		typedef Vector2<unsigned int> Vector2u;
		typedef Vector2<float> Vector2f;
		typedef Vector2<double> Vector2d;

		typedef Vector3<short> Vector3s;
		typedef Vector3<int> Vector3i;
		typedef Vector3<unsigned int> Vector3u;
		typedef Vector3<float> Vector3f;
		typedef Vector3<double> Vector3d;

		typedef Vector4<short> Vector4s;
		typedef Vector4<int> Vector4i;
		typedef Vector4<unsigned int> Vector4u;
		typedef Vector4<float> Vector4f;
		typedef Vector4<double> Vector4d;

		typedef Matrix3<short> Matrix3s;
		typedef Matrix3<int> Matrix3i;
		typedef Matrix3<unsigned int> Matrix3u;
		typedef Matrix3<float> Matrix3f;
		typedef Matrix3<double> Matrix3d;

		typedef Matrix4<short> Matrix4s;
		typedef Matrix4<int> Matrix4i;
		typedef Matrix4<unsigned int> Matrix4u;
		typedef Matrix4<float> Matrix4f;
		typedef Matrix4<double> Matrix4d;

		typedef Quaternion<short> Quaternions;
		typedef Quaternion<int> Quaternioni;
		typedef Quaternion<unsigned int> Quaternionu;
		typedef Quaternion<float> Quaternionf;
		typedef Quaternion<double> Quaterniond;

	}
}

#endif




