#include "JobScheduler.h"

#include "Threads.h"

#include <iostream>
#include <random>

namespace sh
{

namespace jobs
{

	void JobSheduler::Init()
	{
		m_threadsCount = GetHardwareThreadsCount();

		m_isRunning = true;
		m_jobsDoneCount = 0;

		Worker* mainWorker = new Worker(Worker::Type::Foreground, this);
		m_workers.push_back(mainWorker);

		for (int i = 1; i < m_threadsCount; ++i)
		{
			Worker* worker = new Worker(Worker::Type::Background, this);
			m_workers.push_back(worker);
		}

		for (int i = 1; i < m_threadsCount; ++i)
		{
			m_workers[i]->Start();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////

	void JobSheduler::Clear()
	{
		m_isRunning = false;

		for (int i = 0; i < m_threadsCount; ++i)
		{
			m_workers[i]->Stop();
		}

		for (int i = 0; i < m_threadsCount; ++i)
		{
			delete m_workers[i];
		}

		m_workers.clear();
	}

	////////////////////////////////////////////////////////////////////////////////////

	void JobSheduler::AddJob(Job* job)
	{
		GetCurrentThreadWorker()->Submit(job);
	}

	////////////////////////////////////////////////////////////////////////////////////

	Worker* JobSheduler::FindWorker(const std::thread::id id)
	{
		Worker* result = nullptr;
		for (auto worker : m_workers)
		{
			if (worker->GetThreadId() == id)
			{
				result = worker;
				break;
			}
		}

		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////

	Worker* JobSheduler::GetCurrentThreadWorker()
	{
		return FindWorker(std::this_thread::get_id());
	}

	////////////////////////////////////////////////////////////////////////////////////

	Worker* JobSheduler::GetRandomWorker()
	{
		std::uniform_int_distribution<size_t> dist{ 0, m_workers.size() - 1 };
		std::default_random_engine randomEngine{ std::random_device()() };

		size_t index = dist(randomEngine);

		Worker* worker = m_workers[index];
		return worker;

		/*
		if (worker->IsRunning())
		{
			return worker;
		}
		else
		{
			return nullptr;
		}
		*/
	}

	////////////////////////////////////////////////////////////////////////////////////

	void JobSheduler::WaitForCounter(const int counter)
	{
		while (counter != m_jobsDoneCount) {}
	}

} // jobs

} // sh