#pragma once

#include "earthCompileConfig.h"
#include "earthTypes.h"
#include <math.h>
#include <float.h>
#include <stdlib.h> // for abs() etc.
#include <limits.h> // for INT_MAX / UINT_MAX

#ifndef FLT_MAX
#define FLT_MAX 3.402823466E+38F
#endif

#ifndef FLT_MIN
#define FLT_MIN 1.17549435e-38F
#endif

namespace Earth
{
	namespace core
	{
		const s32 ROUNDING_ERROR_S32 = 0;
		const s64 ROUNDING_ERROR_S64 = 0;
		const f32 ROUNDING_ERROR_f32 = 0.000001f;
		const f64 ROUNDING_ERROR_f64 = 0.00000001;

		const f32 PI = 3.14159265359f;
		const f32 RECIPROCAL_PI = 1.0f / PI;
		const f32 HALF_PI = PI / 2.0f;		

		const f32 DEGTORAD = PI / 180.0f;
		const f32 RADTODEG = 180.0f / PI;

		const f64 PI64 = 3.1415926535897932384626433832795028841971693993751;
		const f64 RECIPROCAL_PI64 = 1.0 / PI64;

		const f64 DEGTORAD64 = PI64 / 180.0;
		const f64 RADTODEG64 = 180.0 / PI64;

		inline f32 radToDeg(f32 radians)
		{
			return RADTODEG * radians;
		}

		inline f32 degToRad(f32 degrees)
		{
			return DEGTORAD * degrees;
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

		template<class T>
		inline T abs_(const T& a)
		{
			return a < (T)0 ? -a : a;
		}

		// a -> b 로 t 만큼 변경됨(t==0 이면 a, t==1 이면 b)		
		template<class T>
		inline T lerp(const T& a, const T& b, const f32 t)
		{
			return (T)(a * (1.f - t)) + (b * t);
		}

		//! clamps a value between low and high
		template <class T>
		inline const T clamp(const T& value, const T& low, const T& high)
		{
			return min_(max_(value, low), high);
		}

		template <class T1, class T2>
		inline void swap(T1& a, T2& b)
		{
			T1 c(a);
			a = b;
			b = c;
		}
		
		inline bool equals(const f64 a, const f64 b, const f64 tolerance = ROUNDING_ERROR_f64)
		{
			return (a + tolerance >= b) && (a - tolerance <= b);
		}
		
		inline bool equals(const f32 a, const f32 b, const f32 tolerance = ROUNDING_ERROR_f32)
		{
			return (a + tolerance >= b) && (a - tolerance <= b);
		}		

		inline bool equals(const s32 a, const s32 b, const s32 tolerance = ROUNDING_ERROR_S32)
		{
			return (a + tolerance >= b) && (a - tolerance <= b);
		}

		inline bool equals(const u32 a, const u32 b, const s32 tolerance = ROUNDING_ERROR_S32)
		{
			return (a + tolerance >= b) && (a - tolerance <= b);
		}

		inline bool equals(const s64 a, const s64 b, const s64 tolerance = ROUNDING_ERROR_S64)
		{
			return (a + tolerance >= b) && (a - tolerance <= b);
		}

		inline bool iszero(const f64 a, const f64 tolerance = ROUNDING_ERROR_f64)
		{
			return fabs(a) <= tolerance;
		}

		inline bool iszero(const f32 a, const f32 tolerance = ROUNDING_ERROR_f32)
		{
			return fabsf(a) <= tolerance;
		}

		inline bool isnotzero(const f32 a, const f32 tolerance = ROUNDING_ERROR_f32)
		{
			return fabsf(a) > tolerance;
		}

		inline bool iszero(const s32 a, const s32 tolerance = 0)
		{
			return (a & 0x7ffffff) <= tolerance;
		}

		inline bool iszero(const u32 a, const u32 tolerance = 0)
		{
			return a <= tolerance;
		}

		inline bool iszero(const s64 a, const s64 tolerance = 0)
		{
			return abs_(a) <= tolerance;
		}

		inline s32 s32_min(s32 a, s32 b)
		{
			const s32 mask = (a - b) >> 31;
			return (a & mask) | (b & ~mask);
		}

		inline s32 s32_max(s32 a, s32 b)
		{
			const s32 mask = (a - b) >> 31;
			return (b & mask) | (a & ~mask);
		}

		inline s32 s32_clamp(s32 value, s32 low, s32 high)
		{
			return s32_min(s32_max(value, low), high);
		}

		#define F32_AS_S32(f)		(*((s32 *) &(f)))
		#define F32_AS_U32(f)		(*((u32 *) &(f)))
		#define F32_AS_U32_POINTER(f)	( ((u32 *) &(f)))

		#define F32_VALUE_0		0x00000000
		#define F32_VALUE_1		0x3f800000
		#define F32_SIGN_BIT		0x80000000U
		#define F32_EXPON_MANTISSA	0x7FFFFFFFU

		#ifndef REALINLINE
		#ifdef _MSC_VER
		#define REALINLINE __forceinline
		#else
		#define REALINLINE inline
		#endif
		#endif

		// calculate: sqrt ( x ) : 제곱근을 구한다. ex) sqrt(9) = 3
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

		// calculate: sqrt ( x )
		REALINLINE s64 squareroot(const s64 f)
		{
			return static_cast<s64>(squareroot(static_cast<f64>(f)));
		}

		// calculate: 1 / sqrt ( x )
		REALINLINE f64 reciprocal_squareroot(const f64 x)
		{
			return 1.0 / sqrt(x);
		}

		// calculate: 1 / sqrtf ( x )
		REALINLINE f32 reciprocal_squareroot(const f32 f)
		{
			return 1.f / sqrtf(f);
		}

		// calculate: 1 / sqrtf( x )
		REALINLINE s32 reciprocal_squareroot(const s32 x)
		{
			return static_cast<s32>(reciprocal_squareroot(static_cast<f32>(x)));
		}

		// calculate: 1 / x
		REALINLINE f32 reciprocal(const f32 f)
		{
			return 1.f / f;
		}

		// 내림
		REALINLINE s32 floor32(f32 x)
		{
			return (s32)floorf(x);
		}

		// 올림
		REALINLINE s32 ceil32(f32 x)
		{
			return (s32)ceilf(x);
		}

		// 반올림
		REALINLINE f32 round_(f32 x)
		{
			return floorf(x + 0.5f);
		}

		// 반올림
		REALINLINE s32 round32(f32 x)
		{
			return (s32)round_(x);
		}

		inline f32 f32_max3(const f32 a, const f32 b, const f32 c)
		{
			return a > b ? (a > c ? a : c) : (b > c ? b : c);
		}

		inline f32 f32_min3(const f32 a, const f32 b, const f32 c)
		{
			return a < b ? (a < c ? a : c) : (b < c ? b : c);
		}
	}
}