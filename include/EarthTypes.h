#pragma once

#include "earthCompileConfig.h"

namespace Earth
{
	typedef unsigned __int8		u8;
	typedef __int8				s8;
	typedef char				c8;
	
	typedef unsigned __int16	u16;
	typedef __int16				s16;

	typedef unsigned __int32	u32;
	typedef __int32				s32;

	typedef unsigned __int64	u64;
	typedef __int64				s64;
	
	typedef float				f32;
	typedef double				f64;

// _asm int 3 이란것은 어셈블러인데 브레이크포인트를 의미한다.
#if defined(_DEBUG)
#define _EARTH_DEBUG_BREAK_IF( _CONDITION_ ) if (_CONDITION_) { _asm int 3 }
#else 
#define _EARTH_DEBUG_BREAK_IF( _CONDITION_ )
#endif

//! Defines a deprecated macro which generates a warning at compile time
/** The usage is simple
For typedef:		typedef _IRR_DEPRECATED_ int test1;
For classes/structs:	class _IRR_DEPRECATED_ test2 { ... };
For methods:		class test3 { _IRR_DEPRECATED_ virtual void foo() {} };
For functions:		template<class T> _IRR_DEPRECATED_ void test4(void) {}
**/
#if defined(IGNORE_DEPRECATED_WARNING)
#define _IRR_DEPRECATED_
#elif _MSC_VER >= 1310 //vs 2003 or higher
#define _IRR_DEPRECATED_ __declspec(deprecated)
#elif (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)) // all versions above 3.0 should support this feature
#define _IRR_DEPRECATED_  __attribute__ ((deprecated))
#else
#define _IRR_DEPRECATED_
#endif

}