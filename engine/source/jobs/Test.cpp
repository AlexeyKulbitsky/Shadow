#include "Job.h"
#include "Threads.h"
#include "JobScheduler.h"
#include "Timer.h"

#include "Test.h"

#include <atomic>
#include <iostream>

struct Range
{
	int start;
	int end;

	int result;
};

void Sum(void* args)
{
	Range* range = reinterpret_cast<Range*>(args);
	int res = 0;
	for (int i = range->start; i < range->end; ++i)
	{
		float f = 12.456798f * i;
		float sinF = sinf(f);
		float t = sinF * tanf(sqrt(fabs(sinF)));
		srand(time(nullptr));
		float flt = rand() / 10.12f;
		int a = static_cast<int>(t) * static_cast<int>(flt * 0.000001f);
		res += i + a;
	}
	range->result = res;
}

void TestJobs()
{
	int32_t count = sh::jobs::GetHardwareThreadsCount();
	std::cout << "Threads count: " << count - 1 << std::endl;
	Timer timer;

	// Parallel
	sh::jobs::JobSheduler sheduler;
	sheduler.Init();

	
	timer.Reset();
	timer.Start();
	const int jobsCount = 1024;
	Range ranges[jobsCount];
	sh::jobs::Job jobs[jobsCount];
	for (int i = 0; i < jobsCount; ++i)
	{
		ranges[i].start = 0;
		ranges[i].end = 1024;

		sh::jobs::Job* job = &jobs[i];
		job->args = &ranges[i];
		job->function = Sum;

		sheduler.AddJob(job);
	}
	sheduler.WaitForCounter(jobsCount);
	timer.Stop();
	int parallelRes = 0;
	for (int i = 0; i < jobsCount; ++i)
	{
		parallelRes += ranges[i].result;
	}
	std::cout << "Time parallel: " << timer.GetTimeMS() << std::endl;
	std::cout << "Result in parallel: " << parallelRes << std::endl;
	sheduler.Clear();

	// Linear
	Range linearRange;
	linearRange.start = 0;
	linearRange.end = 1024;
	timer.Reset();
	timer.Start();
	int linearRes = 0;
	for (int i = 0; i < 1024; ++i)
	{
		Sum(&linearRange);
		linearRes += linearRange.result;
	}
	timer.Stop();
	std::cout << "Time linear: " << timer.GetTimeMS() << std::endl;
	std::cout << "Result in linear: " << linearRes << std::endl;
	
	int a = 0;
	std::cin >> a;
}