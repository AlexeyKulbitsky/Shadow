#ifndef SHADOW_JOB_INCLUDE
#define SHADOW_JOB_INCLUDE

namespace sh
{

namespace jobs
{

	typedef void(*JobFunction)(void *args);

	struct Job
	{
		JobFunction function;
		void* args;
	};

} // jobs

} // sh

#endif