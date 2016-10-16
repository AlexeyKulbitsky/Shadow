#ifndef SHADOW_MATH_INCLUDE
#define SHADOW_MATH_INCLUDE

#include "../Types.h"
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <limits.h>

namespace sh
{
	namespace math
	{
		//! Rounding error constant often used when comparing f32 values.

		const s32 EPS_S32 = 0;
#ifdef SH_HAS_S64
		const s64 EPS_S64 = 0;
#endif
		const f32 EPS_f32 = 0.000001f;
		const f64 EPS_f64 = 0.00000001;

#ifdef PI // make sure we don't collide with a define
#undef PI
#endif
		//! Constant for PI.
		const f32 PI = 3.14159265359f;
		const f32 RECIPROCAL_PI = 1.0f / PI;
		const f32 HALF_PI = PI / 2.0f;

#ifdef PI64 // make sure we don't collide with a define
#undef PI64
#endif
		const f64 PI64 = 3.1415926535897932384626433832795028841971693993751;
		const f64 RECIPROCAL_PI64 = 1.0 / PI64;
		const f32 DEGTORAD = PI / 180.0f;
		const f32 RADTODEG = 180.0f / PI;
		const f64 DEGTORAD64 = PI64 / 180.0;
		const f64 RADTODEG64 = 180.0 / PI64;

		inline f32 radToDeg(f32 radians)
		{
			return RADTODEG * radians;
		}

		inline f64 radToDeg(f64 radians)
		{
			return RADTODEG64 * radians;
		}

		inline f32 degToRad(f32 degrees)
		{
			return DEGTORAD * degrees;
		}

		inline f64 degToRad(f64 degrees)
		{
			return DEGTORAD64 * degrees;
		}

		template<class T>
		inline const T& min_(const T& a, const T& b)
		{
			return a < b ? a : b;
		}

		template<class T>
		inline const T& min_(const T& a, const T& b, const T& c)
		{
			return a < b ? min_(a, c) : min_(b, c);
		}

		template<class T>
		inline const T& max_(const T& a, const T& b)
		{
			return a < b ? b : a;
		}

		template<class T>
		inline const T& max_(const T& a, const T& b, const T& c)
		{
			return a < b ? max_(b, c) : max_(a, c);
		}

		//! returns abs of two values. Own implementation to get rid of STL (VS6 problems)
		template<class T>
		inline T abs(const T& a)
		{
			return a < (T)0 ? -a : a;
		}

		template<class T>
		inline T lerp(const T& a, const T& b, const f32 t)
		{
			return (T)(a*(1.f - t)) + (b*t);
		}

		//! clamps a value between low and high
		template <class T>
		inline const T clamp(const T& value, const T& low, const T& high)
		{
			return min(max(value, low), high);
		}

		template <class T>
		inline T sqrt(const T& value)
		{
			return std::sqrt(value);
		}


#ifndef REALINLINE
#ifdef _MSC_VER
#define REALINLINE __forceinline
#else
#define REALINLINE inline
#endif
#endif

#if defined(__BORLANDC__) || defined (__BCPLUSPLUS__)

		// 8-bit bools in borland builder

		//! conditional set based on mask and arithmetic shift
		REALINLINE u32 if_c_a_else_b(const c8 condition, const u32 a, const u32 b)
		{
			return ((-condition >> 7) & (a ^ b)) ^ b;
		}

		//! conditional set based on mask and arithmetic shift
		REALINLINE u32 if_c_a_else_0(const c8 condition, const u32 a)
		{
			return (-condition >> 31) & a;
		}
#else

		//! conditional set based on mask and arithmetic shift
		REALINLINE u32 if_c_a_else_b(const s32 condition, const u32 a, const u32 b)
		{
			return ((-condition >> 31) & (a ^ b)) ^ b;
		}

		//! conditional set based on mask and arithmetic shift
		REALINLINE u16 if_c_a_else_b(const s16 condition, const u16 a, const u16 b)
		{
			return ((-condition >> 15) & (a ^ b)) ^ b;
		}

		//! conditional set based on mask and arithmetic shift
		REALINLINE u32 if_c_a_else_0(const s32 condition, const u32 a)
		{
			return (-condition >> 31) & a;
		}
#endif

		/*
		if (condition) state |= m; else state &= ~m;
		*/
		REALINLINE void setbit_cond(u32 &state, s32 condition, u32 mask)
		{
			// 0, or any postive to mask
			//s32 conmask = -condition >> 31;
			state ^= ((-condition >> 31) ^ state) & mask;
		}

		inline f32 round_(f32 x)
		{
			return floorf(x + 0.5f);
		}

		REALINLINE void clearFPUException()
		{
#ifdef IRRLICHT_FAST_MATH
			return;
#ifdef feclearexcept
			feclearexcept(FE_ALL_EXCEPT);
#elif defined(_MSC_VER)
			__asm fnclex;
#elif defined(__GNUC__) && defined(__x86__)
			__asm__ __volatile__("fclex \n\t");
#endif
#endif
		}

		// calculate: sqrt ( x )
		REALINLINE f32 squareroot(const f32 f)
		{
			return sqrtf(f);
		}

		// calculate: sqrt ( x )
		REALINLINE f64 squareroot(const f64 f)
		{
			return sqrt(f);
		}

		// calculate: sqrt ( x )
		REALINLINE s32 squareroot(const s32 f)
		{
			return static_cast<s32>(squareroot(static_cast<f32>(f)));
		}

#ifdef __IRR_HAS_S64
		// calculate: sqrt ( x )
		REALINLINE s64 squareroot(const s64 f)
		{
			return static_cast<s64>(squareroot(static_cast<f64>(f)));
		}
#endif

		// calculate: 1 / sqrt ( x )
		REALINLINE f64 reciprocal_squareroot(const f64 x)
		{
			return 1.0 / sqrt(x);
		}

		// calculate: 1 / sqrtf ( x )
		REALINLINE f32 reciprocal_squareroot(const f32 f)
		{
#if defined ( IRRLICHT_FAST_MATH )
#if defined(_MSC_VER)
			// SSE reciprocal square root estimate, accurate to 12 significant
			// bits of the mantissa
			f32 recsqrt;
			__asm rsqrtss xmm0, f           // xmm0 = rsqrtss(f)
			__asm movss recsqrt, xmm0       // return xmm0
			return recsqrt;

			/*
			// comes from Nvidia
			u32 tmp = (u32(IEEE_1_0 << 1) + IEEE_1_0 - *(u32*)&x) >> 1;
			f32 y = *(f32*)&tmp;
			return y * (1.47f - 0.47f * x * y * y);
			*/
#else
			return 1.f / sqrtf(f);
#endif
#else // no fast math
			return 1.f / sqrtf(f);
#endif
		}

		// calculate: 1 / sqrtf( x )
		REALINLINE s32 reciprocal_squareroot(const s32 x)
		{
			return static_cast<s32>(reciprocal_squareroot(static_cast<f32>(x)));
		}

		// calculate: 1 / x
		REALINLINE f32 reciprocal(const f32 f)
		{
#if defined (IRRLICHT_FAST_MATH)

			// SSE Newton-Raphson reciprocal estimate, accurate to 23 significant
			// bi ts of the mantissa
			// One Newtown-Raphson Iteration:
			// f(i+1) = 2 * rcpss(f) - f * rcpss(f) * rcpss(f)
#if defined(_MSC_VER)				
			f32 rec;
			__asm rcpss xmm0, f               // xmm0 = rcpss(f)
			__asm movss xmm1, f               // xmm1 = f
			__asm mulss xmm1, xmm0            // xmm1 = f * rcpss(f)
			__asm mulss xmm1, xmm0            // xmm2 = f * rcpss(f) * rcpss(f)
			__asm addss xmm0, xmm0            // xmm0 = 2 * rcpss(f)
			__asm subss xmm0, xmm1            // xmm0 = 2 * rcpss(f)
											  //        - f * rcpss(f) * rcpss(f)
			__asm movss rec, xmm0             // return xmm0
			return rec;
#else // no support yet for other compilers
			return 1.f / f;
#endif
			//! i do not divide through 0.. (fpu expection)
			// instead set f to a high value to get a return value near zero..
			// -1000000000000.f.. is use minus to stay negative..
			// must test's here (plane.normal dot anything ) checks on <= 0.f
			//u32 x = (-(AIR(f) != 0 ) >> 31 ) & ( IR(f) ^ 0xd368d4a5 ) ^ 0xd368d4a5;
			//return 1.f / FR ( x );

#else // no fast math
			return 1.f / f;
#endif
		}

		// calculate: 1 / x
		REALINLINE f64 reciprocal(const f64 f)
		{
			return 1.0 / f;
		}


		// calculate: 1 / x, low precision allowed
		REALINLINE f32 reciprocal_approxim(const f32 f)
		{
#if defined( IRRLICHT_FAST_MATH)

			// SSE Newton-Raphson reciprocal estimate, accurate to 23 significant
			// bi ts of the mantissa
			// One Newtown-Raphson Iteration:
			// f(i+1) = 2 * rcpss(f) - f * rcpss(f) * rcpss(f)
#if defined(_MSC_VER)		
			f32 rec;
			__asm rcpss xmm0, f               // xmm0 = rcpss(f)
			__asm movss xmm1, f               // xmm1 = f
			__asm mulss xmm1, xmm0            // xmm1 = f * rcpss(f)
			__asm mulss xmm1, xmm0            // xmm2 = f * rcpss(f) * rcpss(f)
			__asm addss xmm0, xmm0            // xmm0 = 2 * rcpss(f)
			__asm subss xmm0, xmm1            // xmm0 = 2 * rcpss(f)
											  //        - f * rcpss(f) * rcpss(f)
			__asm movss rec, xmm0             // return xmm0
			return rec;
#else // no support yet for other compilers
			return 1.f / f;
#endif

			/*
			// SSE reciprocal estimate, accurate to 12 significant bits of
			f32 rec;
			__asm rcpss xmm0, f             // xmm0 = rcpss(f)
			__asm movss rec , xmm0          // return xmm0
			return rec;
			*/
			/*
			register u32 x = 0x7F000000 - IR ( p );
			const f32 r = FR ( x );
			return r * (2.0f - p * r);
			*/
#else // no fast math
			return 1.f / f;
#endif
		}


		REALINLINE s32 floor32(f32 x)
		{
#ifdef IRRLICHT_FAST_MATH
			const f32 h = 0.5f;

			s32 t;

#if defined(_MSC_VER)
			__asm
			{
				fld	x
				fsub	h
				fistp	t
			}
#elif defined(__GNUC__)
			__asm__ __volatile__(
				"fsub %2 \n\t"
				"fistpl %0"
				: "=m" (t)
				: "t" (x), "f" (h)
				: "st"
			);
#else
			return (s32)floorf(x);
#endif
			return t;
#else // no fast math
			return (s32)floorf(x);
#endif
		}


		REALINLINE s32 ceil32(f32 x)
		{
#ifdef IRRLICHT_FAST_MATH
			const f32 h = 0.5f;

			s32 t;

#if defined(_MSC_VER)
			__asm
			{
				fld	x
				fadd	h
				fistp	t
			}
#elif defined(__GNUC__)
			__asm__ __volatile__(
				"fadd %2 \n\t"
				"fistpl %0 \n\t"
				: "=m"(t)
				: "t"(x), "f"(h)
				: "st"
			);
#else
			return (s32)ceilf(x);
#endif
			return t;
#else // not fast math
			return (s32)ceilf(x);
#endif
		}



		REALINLINE s32 round32(f32 x)
		{
#if defined(IRRLICHT_FAST_MATH)
			s32 t;

#if defined(_MSC_VER)
			__asm
			{
				fld   x
				fistp t
			}
#elif defined(__GNUC__)
			__asm__ __volatile__(
				"fistpl %0 \n\t"
				: "=m"(t)
				: "t"(x)
				: "st"
			);
#else
			return (s32)round_(x);
#endif
			return t;
#else // no fast math
			return (s32)round_(x);
#endif
		}

		inline f32 f32_max3(const f32 a, const f32 b, const f32 c)
		{
			return a > b ? (a > c ? a : c) : (b > c ? b : c);
		}

		inline f32 f32_min3(const f32 a, const f32 b, const f32 c)
		{
			return a < b ? (a < c ? a : c) : (b < c ? b : c);
		}

		inline f32 fract(f32 x)
		{
			return x - floorf(x);
		}

	}
}

#endif
