#pragma once

#include "vector2d.h"

namespace Earth
{
	namespace core
	{

		// Use typedefs where possible as they are more explicit...

		//! \deprecated position2d is now a synonym for vector2d, but vector2d should be used directly.
		typedef vector2d<f32> position2df;

		//! \deprecated position2d is now a synonym for vector2d, but vector2d should be used directly.
		typedef vector2d<s32> position2di;
	} // namespace core
} // namespace irr

// ...and use a #define to catch the rest, for (e.g.) position2d<f64>
#define position2d vector2d
