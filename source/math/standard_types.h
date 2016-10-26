#pragma once

#if (defined(_WIN32) || defined(_WIN64))

namespace sh
{
	//put them in a namespace to avoid conflicts with other libs that might do the same
	namespace math
	{
// 		typedef unsigned char uint8_t;
// 		typedef signed char int8_t;
// 
// 		typedef unsigned short uint16_t;
// 		typedef short int16_t;
// 
// 		typedef unsigned __int32 uint32_t;
// 		typedef __int32 int32_t;
// 
// 		typedef unsigned __int64 uint64_t;
// 		typedef __int64 int64_t;

	}

}

#else
#	include <stdint.h>
#endif