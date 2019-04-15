#pragma once

#include "common/PlatformDefines.h"

#if defined (SHADOW_WINDOWS)
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

#include <assert.h>
#include <cstdint>
#include <sstream>
#include <iostream>

