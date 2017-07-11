#include "AndroidDevice.h"
#include "../../video/Driver.h"
#include "../../video/GLContext/EGLContextManager.h"
#include "../../video/GLES20/GLES20Driver.h"
#include "../../video/Vulkan/VulkanDriver.h"
#include "../../scene/SceneManager.h"
#include "../../io/android/AndroidFileSystem.h"

using namespace sh;
using namespace video;


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
	//m_GLContextManager->InitContext(m_creationParameters);
	//m_driver->Init();

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
	case video::DriverType::VULKAN:
	{
		m_driver = new video::VulkanDriver(m_creationParameters);
	}
	break;
	default:
		break;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////