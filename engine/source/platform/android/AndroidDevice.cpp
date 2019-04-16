#include "platform/android/AndroidDevice.h"
#include "device/Application.h"
#include "video/GL/GLES20/GLES20RenderSystem.h"
#include "video/GL/GLContext/EGLContextManager.h"

namespace sh
{
	AndroidCreationParams AndroidDevice::params;

	AndroidDevice::AndroidDevice()
	{
	}

	AndroidDevice::AndroidDevice(const CreationParameters &parameters)
		:Device(parameters)
	{
	}

	AndroidDevice::~AndroidDevice()
	{
	}

	void AndroidDevice::Init()
	{
		Device::Init();

		// Rendering API intialization
		switch(m_creationParameters.driverType)
		{
			case DriverType::OpenGL_ES_2_0:
			{
				video::RenderSystem::CreateInstance<video::GLES20RenderSystem>();
			}
			break;
			default:
				break;
		}
		video::RenderSystem* renderSystemInstance = video::RenderSystem::GetInstance();
		assert(renderSystemInstance != nullptr && "Can not create RenderSystem instance");
		renderSystemInstance->Init();

		m_application->Init();
	}

	void AndroidDevice::Update(float)
	{
	}

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
					m_application->Destroy();
					return;
				}

				ProcessEvent(event);
			}

			const uint64_t currentTimePoint = GetTime();
			const uint64_t delta = currentTimePoint - m_lastFrameTimePoint;

			m_application->Update(delta);

			m_lastFrameTimePoint = currentTimePoint;
		}
	}

	uint64_t AndroidDevice::GetTime()
	{
		uint64_t result = 0;
		return result;
	}

	void AndroidDevice::OnEvent(const AndroidEvent& event)
	{
		m_eventsQueue.Push(event);
	}

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
			auto renderSystem = video::RenderSystem::GetInstance();
			switch(m_creationParameters.driverType)
			{
				case DriverType::OpenGL_ES_2_0:
				{
					auto gles20RenderSystem = static_cast<video::GLES20RenderSystem*>(renderSystem);
					auto contextManager = gles20RenderSystem->GetContextManager();
					auto eglContextManager = static_cast<video::EGLContextManager*>(contextManager);
					eglContextManager->CreateContext(ev.event.surfaceChangedEvent.winId);
				}
				break;
				default:
					break;
			}

			m_creationParameters.width = ev.event.surfaceChangedEvent.width;
			m_creationParameters.height = ev.event.surfaceChangedEvent.height;
			m_creationParameters.WinId = ev.event.surfaceChangedEvent.winId;

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
}



