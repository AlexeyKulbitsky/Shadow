#ifndef SHADOW_JOB_SCHEDULER_INCLUDE
#define SHADOW_JOB_SCHEDULER_INCLUDE

#include "Job.h"
#include "Worker.h"

#include <vector>
#include <thread>
#include <atomic>

namespace sh
{

namespace jobs
{
	struct ThreadArgs;

	class JobSheduler
	{
	public:
		void Init();
		void Clear();
		void AddJob(Job* job);

		Worker* FindWorker(const std::thread::id id);
		Worker* GetCurrentThreadWorker();
		Worker* GetRandomWorker();

		void WaitForCounter(const int counter);


	public:
		int32_t m_threadsCount = 0U;

		std::atomic_bool m_isRunning;
		std::atomic_int m_jobsDoneCount;

		std::vector<Worker*> m_workers;
	};

	

} // jobs

} // sh

#endif