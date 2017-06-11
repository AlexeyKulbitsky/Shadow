#include "JobQueue.h"

#include  <algorithm>

namespace sh
{

namespace jobs
{
	JobQueue::JobQueue(size_t maxJobs)
	{
		m_jobs.resize(maxJobs);
		m_top = 0;
		m_bottom = 0;
	}

	////////////////////////////////////////////////////////////////////////

	bool JobQueue::Push(Job* job)
	{
		int bottom = m_bottom.load(std::memory_order_acquire);

		if (bottom < static_cast<int>(m_jobs.size()))
		{
			m_jobs[bottom] = job;
			m_bottom.store(bottom + 1, std::memory_order_release);

			return true;
		}
		else
		{
			return false;
		}
	}

	////////////////////////////////////////////////////////////////////////

	Job* JobQueue::Pop()
	{
		int bottom = m_bottom.load(std::memory_order_acquire);

		bottom = std::max(0, bottom - 1);
		m_bottom.store(bottom, std::memory_order_release);
		int top = m_top.load(std::memory_order_acquire);

		if (top <= bottom)
		{
			Job* job = m_jobs[bottom];

			if (top != bottom)
			{
				// More than one job left in the queue
				return job;
			}
			else
			{
				int expectedTop = top;
				int desiredTop = top + 1;

				if (!m_top.compare_exchange_weak(expectedTop, desiredTop,
					std::memory_order_seq_cst))
				{
					// Someone already took the last item, abort
					job = nullptr;
				}

				m_bottom.store(top + 1, std::memory_order_release);
				return job;
			}
		}
		else
		{
			// Queue already empty
			//m_bottom.store(top, std::memory_order_release);
			m_bottom.store(0, std::memory_order_release);
			m_top.store(0, std::memory_order_release);

			return nullptr;
		}
	}

	////////////////////////////////////////////////////////////////////////

	Job* JobQueue::Steal()
	{
		int top = m_top.load(std::memory_order_acquire);

		int bottom = m_bottom.load(std::memory_order_acquire);

		if (top < bottom) 
		{
			Job* job = m_jobs[top];

			int expectedTop = top;
			int desiredTop = top + 1;
			if (!m_top.compare_exchange_weak(expectedTop, desiredTop, std::memory_order_seq_cst))
			{
				job = nullptr;
			}

			return job;
		}

		return nullptr;
	}

	////////////////////////////////////////////////////////////////////////

	size_t JobQueue::GetSize() const
	{
		return m_jobs.size();
	}

	////////////////////////////////////////////////////////////////////////

	bool JobQueue::IsEmpty() const
	{
		return m_bottom.load(std::memory_order_relaxed) == m_top.load(std::memory_order_relaxed);
	}

	////////////////////////////////////////////////////////////////////////


} // jobs

} // sh