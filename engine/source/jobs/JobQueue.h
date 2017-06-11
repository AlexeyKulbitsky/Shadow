#ifndef SHADOW_JOB_QUEUE_INCLUDE
#define SHADOW_JOB_QUEUE_INCLUDE

#include <vector>
#include <atomic>

namespace sh
{

namespace jobs
{

	struct Job;

	class JobQueue
	{
	public:
		JobQueue(size_t maxJobs);

		bool Push(Job* job);
		Job* Pop();
		Job* Steal();
		size_t GetSize() const;
		bool IsEmpty() const;

	private:
		std::vector<Job*> m_jobs;
		std::atomic<int> m_top;
		std::atomic<int> m_bottom;
	};

} // jobs

} // sh

#endif