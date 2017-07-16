#ifndef SHADOW_ANDROID_EVENTS_QUEUE_INCLUDE
#define SHADOW_ANDROID_EVENTS_QUEUE_INCLUDE

#include "../../Globals.h"

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace sh
{
	struct AndroidEvent
	{
		enum class Type
		{
			SurfaceChanged,
			Resize,
            Exit
		};

		Type type;

		union Event
		{
			struct
			{
				int width;
				int height;
			} resizeEvent; 

			struct
			{
				int width;
				int height;
				void* winId;
			} surfaceChangedEvent;


		} event;
	};

    class AndroidEventsQueue
    {
	public:
        AndroidEvent Pop()
        {
			AndroidEvent ev = m_queue.front();
			m_queue.pop();
			return ev;
        }

        void Push(const AndroidEvent& event)
        {
			m_mutex.lock();
			m_queue.push(event);
			m_mutex.unlock();
        }

		bool IsEmpty() const { return m_queue.empty(); }

	private:
		std::queue<AndroidEvent> m_queue;
		std::mutex m_mutex;
		std::condition_variable m_condition;

    };


} // sh

#endif
