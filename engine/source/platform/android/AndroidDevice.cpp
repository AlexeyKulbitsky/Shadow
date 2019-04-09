#include "platform/android/AndroidDevice.h"
#include "Application.h"

using namespace sh;


////////////////////////////////////////////////////////////////////////

AndroidCreationParams AndroidDevice::params;

////////////////////////////////////////////////////////////////////////

AndroidDevice::AndroidDevice()
{
}

////////////////////////////////////////////////////////////////////////

AndroidDevice::AndroidDevice(const CreationParameters &parameters)
	:Device(parameters)
{
}

////////////////////////////////////////////////////////////////////////

AndroidDevice::~AndroidDevice()
{
}

////////////////////////////////////////////////////////////////////////

void AndroidDevice::Init()
{
	Device::Init();
    m_application->Init();
}

////////////////////////////////////////////////////////////////////////

void AndroidDevice::Update(float)
{
}

////////////////////////////////////////////////////////////////////////

void AndroidDevice::Run()
{
	bool done = false;
	while (!done)
	{
		while (!m_eventsQueue.IsEmpty())
		{
			auto event = m_eventsQueue.Pop();
			if (event.type == AndroidEvent::Type::Exit)
			{
				done = true;
				break;
			}

			ProcessEvent(event);
		}

		const uint64_t currentTimePoint = GetTime();
		const uint64_t delta = currentTimePoint - m_lastFrameTimePoint;

		m_application->Update(delta);

		m_lastFrameTimePoint = currentTimePoint;
	}
}

////////////////////////////////////////////////////////////////////////

uint64_t AndroidDevice::GetTime()
{
	uint64_t result = 0;
	return result;
}

////////////////////////////////////////////////////////////////////////

void AndroidDevice::OnEvent(const AndroidEvent& event)
{
	m_eventsQueue.Push(event);
}

////////////////////////////////////////////////////////////////////////

void AndroidDevice::ProcessEvent(const AndroidEvent& ev)
{
	switch (ev.type)
	{
	case AndroidEvent::Type::Resize:
	{
	}
		break;
	case AndroidEvent::Type::SurfaceChanged:
	{
	}
		break;
    case AndroidEvent::Type::Press:
    {
    }
        break;
    case AndroidEvent::Type::Release:
    {
    }
        break;
    case AndroidEvent::Type::Move:
    {
    }
        break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////