#pragma once

#ifdef WIN32
#include <xmmintrin.h>
#endif

//needed for std::min
#include <algorithm>
//needed for std::cos
#include <cmath>
#include <cstdlib>// needed to disambiguate std::abs<int> in xcode5
// needed for memcmp on Linux
#include <cstring>

#include <assert.h>

//Copied from Jet - use these ones instead of PTODO
//Remember - math is independent of Jet
#if defined(_DEBUG)
#	define MATH_STRINGIZE(X) MATH_DO_STRINGIZE(X)
#	define MATH_DO_STRINGIZE(X) #X
#	define MTODO_MESSAGE_STRING(msg) __FILE__ "(" MATH_STRINGIZE(__LINE__) ") : TODO - " msg " - [ " __FUNCTION__ " ]" 
#	if defined(_MSC_VER) // Pragma messages are only supported on MSVC...
#		define MTODO(msg) __pragma( message( MTODO_MESSAGE_STRING(msg) ) )
#		define MDEPRECATED(msg) MTODO("[DEPRECATED]" msg)
#	elif defined(__GNUC__) && defined(GCC_VERSION) && GCC_VERSION >= 40400  // And GCC > 4.4.x 
#		define MTODO_MESSAGE_GNUC(msg) _Pragma(#msg)
#		define MTODO(msg) MTODO_MESSAGE_GNUC( message( MTODO_MESSAGE_STRING(msg) ) )
#		define MDEPRECATED(msg) MTODO("[DEPRECATED]"msg)
#	else
#		define MTODO(msg)
#		define MDEPRECATED(msg)
#	endif
#else
#	define MTODO(msg)
#	define MDEPRECATED(msg)
#endif

#if defined(__ARM_NEON__) && !defined(__arm64__) 
#define MATH_USE_NEON
#endif

#include "constants.h"
#include "standard_types.h"

// include files order matters, mat4 requires vec3 and vec4, watch out!
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "rect.h"//requires vec2
#include "mat2.h"
#include "mat3.h"
#include "mat4.h"
#include "quat.h"
#include "plane.h"
#include "sphere.h"
#include "line2.h"
#include "line3.h"
#include "triangle3.h"
#include "aabb.h"
#include "aabb2.h"
#include "color.h"

#include "func.h"
#include "func_test.h"
#include "func_range.h"
#include "func_trig.h"
#include "func_common.h"
#include "func_interp.h"
#include "func_projection.h"
#include "func_transform.h"
#include "func_binary.h"
#include "func_static_constructors.h"
#include "func_rayintersect.h"
#include "func_circleintersect.h"
#include "cast.h"
#include "angle.h"

namespace sh
{
	namespace math
	{
		typedef vec2<int16_t> vec2s;
		typedef vec2<int32_t> vec2i;
		typedef vec2<uint32_t> vec2u;
		typedef vec2<float> vec2f;
		typedef vec2<double> vec2d;

		typedef vec3<int16_t> vec3s;
		typedef vec3<int32_t> vec3i;
		typedef vec3<uint32_t> vec3u;
		typedef vec3<float> vec3f;
		typedef vec3<double> vec3d;

		typedef vec4<int16_t> vec4s;
		typedef vec4<int32_t> vec4i;
		typedef vec4<uint32_t> vec4u;
		typedef vec4<float> vec4f;
		typedef vec4<double> vec4d;

		typedef quat<int16_t> quats;
		typedef quat<int32_t> quati;
		typedef quat<uint32_t> quatu;
		typedef quat<float> quatf;
		typedef quat<double> quatd;

		typedef mat2<float> mat2f;
		typedef mat2<double> mat2d;

		typedef mat3<float> mat3f;
		typedef mat3<double> mat3d;

		typedef mat4<float> mat4f;
		typedef mat4<double> mat4d;
		 
		typedef line2<float> line2f;
		typedef line2<double> line2d;
		 
		typedef line3<float> line3f;
		typedef line3<double> line3d;
		 
		typedef rect<int32_t> recti;
		typedef rect<uint32_t> rectu;
		typedef rect<float> rectf;
		 
		typedef aabb<float>	 aabbf;
		typedef aabb<double> aabbd;
		 
		typedef aabb2<float> aabb2f;
		typedef aabb2<double> aabb2d;
		 
		typedef triangle3<float>  triangle3f;
		typedef triangle3<double>  triangle3d;
	}
}

#include "func.inl"
#include "func_test.inl"
#include "func_range.inl"
#include "func_trig.inl"
#include "func_common.inl"
#include "func_interp.inl"
#include "func_projection.inl"
#include "func_transform.inl"
#include "func_binary.inl"
#include "func_static_constructors.inl"
#include "vec2.inl"
#include "vec3.inl"
#include "vec4.inl"
#include "rect.inl"
#include "mat2.inl"
#include "mat3.inl"
#include "mat4.inl"
#include "quat.inl"
#include "plane.inl"
#include "sphere.inl"
#include "line2.inl"
#include "line3.inl"
#include "triangle3.inl"
#include "aabb.inl"
#include "aabb2.inl"
#include "func_rayintersect.inl"
#include "func_circleintersect.inl"
#include "color.inl"
#include "cast.inl"
#include "angle.inl"



