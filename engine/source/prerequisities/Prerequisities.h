#pragma once

#include "prerequisities/PlatformDefines.h"

#if defined (SHADOW_WINDOWS)
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

#include "prerequisities/Forward.h"

#include <assert.h>
#include <cstdint>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>

