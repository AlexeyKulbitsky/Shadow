#include "AndroidDevice.h"
#include "../../video/Driver.h"
#include "../../video/GLContext/EGLContextManager.h"
#include "../../video/GLES20/GLES20Driver.h"

#if defined (SHADOW_USE_VULKAN_API)
#include "../../video/Vulkan/VulkanDriver.h"
#endif

#include "../../scene/SceneManager.h"
#include "../../scene/Camera.h"
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

    io::AndroidFileSystem* androidFileSystem = static_cast<io::AndroidFileSystem*>(m_fileSystem);
	androidFileSystem->SetApkPath(params.apkPath);
	CreateDriver();
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
	m_driver->SetWindow(params.WinId, params.width, params.height);
	m_driver->Init();

	Device::Init();

    m_sceneManager->GetCamera()->SetProjection(3.1415926535f / 3.0f,
                                               static_cast<f32>(params.width),
                                               static_cast<f32>(params.height), 0.1f, 1000.0f);

    m_application->Init();
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
		sursafeChangedEvent(
                ev.event.surfaceChangedEvent.winId,
                ev.event.surfaceChangedEvent.width,
			ev.event.surfaceChangedEvent.height);
	}
		break;
    case AndroidEvent::Type::Press:
    {
        mouseEvent(ev.event.pressEvent.x,
                   ev.event.pressEvent.y,
                   MouseEventType::ButtonPressed, MouseCode::ButtonLeft);
    }
        break;
    case AndroidEvent::Type::Release:
    {
        mouseEvent(ev.event.releaseEvent.x,
                   ev.event.releaseEvent.y,
                   MouseEventType::ButtonReleased, MouseCode::ButtonLeft);
    }
        break;
    case AndroidEvent::Type::Move:
    {
        mouseEvent(ev.event.moveEvent.x,
                   ev.event.moveEvent.y,
                   MouseEventType::Moved, MouseCode::ButtonLeft);
    }
        break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////