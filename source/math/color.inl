namespace sh
{
	namespace math
	{
		namespace color
		{


			template<typename T>
			inline vec4<T> u32ToRgbaVec4(uint32_t v)
			{
				return vec4<T>(vec4<uint32_t>((v >> 16) & 0xFF, (v >> 8) & 0xFF, (v) & 0xFF, (v >> 24) & 0xFF)) * (T)0.003921568627451;
			}
			template<typename T>
			inline vec4<T> u32ToArgbVec4(uint32_t v)
			{
				return vec4<T>(vec4<uint32_t>((v >> 24) & 0xFF, (v >> 16) & 0xFF, (v >> 8) & 0xFF, (v) & 0xFF)) * (T)0.003921568627451;
			}
			template<typename T>
			inline vec4<T> u32ToBgraVec4(uint32_t v)
			{
				return vec4<T>(vec4<uint32_t>((v) & 0xFF, (v >> 8) & 0xFF, (v >> 16) & 0xFF, (v >> 24) & 0xFF)) * (T)0.003921568627451;
			}
			template<typename T>
			inline vec4<T> u32ToAbgrVec4(uint32_t v)
			{
				return vec4<T>(vec4<uint32_t>((v >> 24) & 0xFF, (v) & 0xFF, (v >> 8) & 0xFF, (v >> 16) & 0xFF)) * (T)0.003921568627451;
			}

			template<typename T>
			inline uint32_t rgbaVec4ToU32(const vec4<T>& v)
			{
				vec4<uint32_t> uv(v * (T)255);
				return ((uv.w & 0xFF) << 24) | ((uv.x & 0xFF) << 16) | ((uv.y & 0xFF) << 8) | ((uv.z & 0xFF));
			}
			template<typename T>
			inline uint32_t argbVec4ToU32(const vec4<T>& v)
			{
				vec4<uint32_t> uv(v * (T)255);
				return ((uv.x & 0xFF) << 24) | ((uv.y & 0xFF) << 16) | ((uv.z & 0xFF) << 8) | ((uv.w & 0xFF));
			}
			template<typename T>
			inline uint32_t bgraVec4ToU32(const vec4<T>& v)
			{
				vec4<uint32_t> uv(v * (T)255);
				return ((uv.w & 0xFF) << 24) | ((uv.z & 0xFF) << 16) | ((uv.y & 0xFF) << 8) | ((uv.x & 0xFF));
			}
			template<typename T>
			inline uint32_t abgrVec4ToU32(const vec4<T>& v)
			{
				vec4<uint32_t> uv(v * (T)255);
				return ((uv.x & 0xFF) << 24) | ((uv.w & 0xFF) << 16) | ((uv.z & 0xFF) << 8) | ((uv.y & 0xFF));
			}

			//////////////////////////////////////////////////////////////////////////

			template<typename T>
			inline vec3<T> u32ToRgbVec3(uint32_t v)
			{
				return vec3<T>(vec3<uint32_t>((v >> 16) & 0xFF, (v >> 8) & 0xFF, (v) & 0xFF)) * (T)0.003921568627451;
			}
			template<typename T>
			inline vec3<T> u32ToBgrVec3(uint32_t v)
			{
				return vec3<T>(vec3<uint32_t>((v) & 0xFF, (v >> 8) & 0xFF, (v >> 16) & 0xFF)) * (T)0.003921568627451;
			}

			template<typename T>
			inline uint32_t rgbVec3ToU32(const vec3<T>& v)
			{
				vec3<uint32_t> uv(v * (T)255);
				return ((uv.x & 0xFF) << 16) | ((uv.y & 0xFF) << 8) | ((uv.z & 0xFF)) | (0xFFu << 24);
			}

			template<typename T>
			inline uint32_t bgrVec3ToU32(const vec3<T>& v)
			{
				vec3<uint32_t> uv(v * (T)255);
				return ((uv.z & 0xFF) << 16) | ((uv.y & 0xFF) << 8) | ((uv.x & 0xFF)) | (0xFFu << 24);
			}

			inline uint32_t argbToAbgr(uint32_t v)
			{
				uint32_t a = (v) >> 24;
				uint32_t r = (v >> 16) & 0xFF;
				uint32_t g = (v >> 8) & 0xFF;
				uint32_t b = (v) & 0xFF;
				return (a << 24) | (b << 16) | (g << 8) | (r);
			}
			inline uint32_t abgrToArgb(uint32_t v)
			{
				uint32_t a = (v) >> 24;
				uint32_t b = (v >> 16) & 0xFF;
				uint32_t g = (v >> 8) & 0xFF;
				uint32_t r = (v) & 0xFF;
				return (a << 24) | (r << 16) | (g << 8) | (b);
			}
			inline uint32_t rgbaToBgra(uint32_t v)
			{
				uint32_t r = (v >> 24) & 0xFF;
				uint32_t g = (v >> 16) & 0xFF;
				uint32_t b = (v >> 8) & 0xFF;
				uint32_t a = (v) & 0xFF;
				return (b << 24) | (g << 16) | (r << 8) | (a);
			}

			inline uint32_t bgraToRgba(uint32_t v)
			{
				uint32_t b = (v >> 24) & 0xFF;
				uint32_t g = (v >> 16) & 0xFF;
				uint32_t r = (v >> 8) & 0xFF;
				uint32_t a = (v) & 0xFF;
				return (r << 24) | (g << 16) | (b << 8) | (a);
			}

			inline uint32_t swapRB(uint32_t v)
			{
				uint32_t x = (v >> 16) & 0xFF;
				uint32_t y = v & 0xFF;
				return (v & 0xFF00FF00) | (y << 16) | x;
			}

			inline uint32_t lerp(uint32_t a, uint32_t b, uint32_t factor)
			{
				uint32_t af = 256 - factor;
				uint32_t bf = factor;

				// uint32_t abgr -> 0a0g0b0r
				uint32_t al = (a & 0x00ff00ff);
				uint32_t ah = (a & 0xff00ff00) >> 8;
				uint32_t bl = (b & 0x00ff00ff);
				uint32_t bh = (b & 0xff00ff00) >> 8;

				// -> a_g_b_r_
				uint32_t rl = (al * af + bl * bf);
				uint32_t rh = (ah * af + bh * bf);

				// shift & combine back into 32-bit quantity
				uint32_t result = (rh & 0xff00ff00) | ((rl & 0xff00ff00) >> 8);
				return result;
			}

			inline uint32_t multiply(uint32_t x, uint32_t y)
			{
				/*

				// this code is not safe because of the possible overflow
				// 0x00ff00ff * 0x00ff00ff is much greater than 0xffffffff

				// uint32_t rl = al * bl;
				// uint32_t rh = ah * bh;

				// uint32_t abgr -> 0a0g0b0r
				uint32_t al = (a & 0x00ff00ff);
				uint32_t ah = (a & 0xff00ff00) >> 8;
				uint32_t bl = (b & 0x00ff00ff);
				uint32_t bh = (b & 0xff00ff00) >> 8;

				// -> a_g_b_r_
				uint32_t rl = al * bl;
				uint32_t rh = ah * bh;
				// shift & combine back into 32-bit quantity
				uint32_t result = (rh & 0xff00ff00) | ((rl & 0xff00ff00) >> 8);
				*/

				// using fixed point arithmetic
				// precision error is 1/255 = 0.0039
				// 0xff * 0xff = 0xfe

				uint32_t xa = (x) >> 24;
				uint32_t xb = (x >> 16) & 0xFF;
				uint32_t xg = (x >> 8) & 0xFF;
				uint32_t xr = (x) & 0xFF;

				uint32_t ya = (y) >> 24;
				uint32_t yb = (y >> 16) & 0xFF;
				uint32_t yg = (y >> 8) & 0xFF;
				uint32_t yr = (y) & 0xFF;

				uint32_t a = (xa * ya) >> 8;
				uint32_t b = (xb * yb) >> 8;
				uint32_t g = (xg * yg) >> 8;
				uint32_t r = (xr * yr) >> 8;

				uint32_t result = (a << 24) | (b << 16) | (g << 8) | (r);

				return result;
			}


		}
	}
}