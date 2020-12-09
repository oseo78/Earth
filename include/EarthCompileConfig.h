#pragma once

#define EARTH_SDK_VERSION "1.0.0"

#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#define _EARTH_WINDOWS_
#define _EARTH_WINDOWS_API_
#define _EARTH_COMPILE_WITH_WINDOWS_DEVICE_
#endif

#if defined(_WIH32_WCE)
# error "Mobile devices not supported."
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1300)
# error "Only Microsoft Visual Studio 7.0 and later are supported."
#endif

#if defined(_XBOX)
# error "XBOX devices not supported."
#endif

#if defined(__APPLE__) || defined(MACOSX)
# error "Mac OS not supported."
#endif

#define _EARTH_COMPILE_WITH_DIRECT3D_9_

#define IRRLICHT_API __declspec(dllexport)