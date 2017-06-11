#ifndef SHADOW_THREADS_INCLUDE
#define SHADOW_THREADS_INCLUDE

#include <thread>


namespace sh
{

namespace jobs
{

	inline int32_t GetHardwareThreadsCount()
	{
		int32_t res = std::thread::hardware_concurrency();
		return res;
	}

} // jobs

} // sh

#endif