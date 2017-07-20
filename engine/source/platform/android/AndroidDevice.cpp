#include "AndroidDevice.h"
#include "../../video/Driver.h"
#include "../../video/GLContext/EGLContextManager.h"
#include "../../video/GLES20/GLES20Driver.h"

#if defined (SHADOW_USE_VULKAN_API)
#include "../../video/Vulkan/VulkanDriver.h"
#endif

#include "../../scene/SceneManager.h"
#include "../../io/android/AndroidFileSystem.h"
#include "../../Application.h"

using namespace sh;
using namespace video;


////////////////////////////////////////////////////////////////////////

AndroidCreationParams AndroidDevice::params;

////////////////////////////////////////////////////////////////////////

AndroidDevice::AndroidDevice()
{
#if defined SHADOW_ANDROID
	io::FileSystem::CreateInstance<io::AndroidFileSystem>();
	m_fileSystem = io::FileSystem::GetInstance();
#endif
}

////////////////////////////////////////////////////////////////////////

AndroidDevice::AndroidDevice(const CreationParameters &parameters)
	:Device(parameters)
{
	
	if (!m_creationParameters.WinId)
	{
		;
	}
	else if (m_creationParameters.WinId)
	{
		;
	}

#if defined SHADOW_ANDROID
	io::FileSystem::CreateInstance<io::AndroidFileSystem>();
	m_fileSystem = io::FileSystem::GetInstance();
#endif

	CreateDriver();
}

////////////////////////////////////////////////////////////////////////

AndroidDevice::~AndroidDevice()
{

}

////////////////////////////////////////////////////////////////////////

void AndroidDevice::Init()
{
	m_driver->SetWindow(m_creationParameters.WinId, m_creationParameters.width, m_creationParameters.height);
	m_driver->Init();

	Device::Init();
}

////////////////////////////////////////////////////////////////////////

void AndroidDevice::Update(f32 deltaTime)
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

		const u64 currentTimePoint = GetTime();
		const u64 delta = currentTimePoint - m_lastFrameTimePoint;

		m_application->Update(delta);

		m_lastFrameTimePoint = currentTimePoint;
	}
}

////////////////////////////////////////////////////////////////////////

u64 AndroidDevice::GetTime()
{
	u64 result = 0;
	return result;
}

////////////////////////////////////////////////////////////////////////

bool AndroidDevice::CreateDriver()
{
	switch (m_creationParameters.driverType)
	{
	case video::DriverType::OPENGL_ES_2_0:
	{
		video::EGLContextManager* contextManager = new video::EGLContextManager();
		if (contextManager)
		{
			m_driver = new video::GLES20Driver(contextManager);
			m_GLContextManager = contextManager;
		}
	}
	break;
#if defined (SHADOW_USE_VULKAN_API)
	case video::DriverType::VULKAN:
	{
		m_driver = new video::VulkanDriver(m_creationParameters);
	}
	break;
#endif
	default:
		break;
	}
	return true;
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
		windowResizeEvent(ev.event.resizeEvent.width, ev.event.resizeEvent.height);
	}
		break;
	case AndroidEvent::Type::SurfaceChanged:
	{
		sursafeChangedEvent(ev.event.surfaceChangedEvent.width,
			ev.event.surfaceChangedEvent.height,
			ev.event.surfaceChangedEvent.winId);
	}
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////