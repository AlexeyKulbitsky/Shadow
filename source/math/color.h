#pragma once
namespace sh
{


	namespace math
	{
		namespace color
		{

			template<typename T>
			vec4<T> u32ToRgbaVec4(uint32_t);

			template<typename T>
			vec4<T> u32ToArgbVec4(uint32_t);

			template<typename T>
			vec4<T> u32ToBgraVec4(uint32_t);

			template<typename T>
			vec4<T> u32ToAbgrVec4(uint32_t);

			template<typename T>
			uint32_t rgbaVec4ToU32(const vec4<T>& v);

			template<typename T>
			uint32_t argbVec4ToU32(const vec4<T>& v);

			template<typename T>
			uint32_t bgraVec4ToU32(const vec4<T>& v);

			template<typename T>
			uint32_t abgrVec4ToU32(const vec4<T>& v);


			template<typename T>
			vec3<T> u32ToRgbVec3(uint32_t);

			template<typename T>
			vec3<T> u32ToBgrVec3(uint32_t);

			template<typename T>
			uint32_t rgbVec3ToU32(const vec3<T>& v);

			template<typename T>
			uint32_t bgrVec3ToU32(const vec3<T>& v);

			uint32_t argbToAbgr(uint32_t v);
			uint32_t abgrToArgb(uint32_t v);
			uint32_t rgbaToBgra(uint32_t v);
			uint32_t bgraToRgba(uint32_t v);

			uint32_t swapRB(uint32_t v);

			uint32_t lerp(uint32_t a, uint32_t b, uint32_t t);
			uint32_t multiply(uint32_t a, uint32_t b);
		}
	}

}
