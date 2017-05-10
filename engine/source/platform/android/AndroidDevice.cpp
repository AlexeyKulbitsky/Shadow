#include "AndroidDevice.h"
#include "../../video/Driver.h"
#include "../../video/GLContext/EGLContextManager.h"
#include "../../video/GLES20/GLES20Driver.h"
#include "../../scene/SceneManager.h"

using namespace sh;
using namespace video;


AndroidDevice::AndroidDevice()
{

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

	CreateDriver();
}

////////////////////////////////////////////////////////////////////////

AndroidDevice::~AndroidDevice()
{

}

////////////////////////////////////////////////////////////////////////

void AndroidDevice::Init()
{
	m_GLContextManager->InitContext(m_creationParameters);
	m_driver->Init();
}

////////////////////////////////////////////////////////////////////////

void AndroidDevice::Update(f32 deltaTime)
{
	
}

////////////////////////////////////////////////////////////////////////

bool AndroidDevice::Run()
{
	return true;
}

////////////////////////////////////////////////////////////////////////

/*
void AndroidDevice::OnEvent(const Event& e)
{
	switch (e.type)
	{
	case EventType::MOUSE_INPUT_EVENT:
	{
		m_inputManager->OnEvent(e);
	}
		break;
	case EventType::KEYBOARD_INPUT_EVENT:
	{
		m_inputManager->OnEvent(e);
	}
		break;
	default:
		break;
	}
}
*/
////////////////////////////////////////////////////////////////////////

u64 AndroidDevice::GetTime()
{
	//static LARGE_INTEGER freq;
	//static LARGE_INTEGER crt;
	//QueryPerformanceFrequency(&freq);
	//QueryPerformanceCounter(&crt);
	//return (crt.QuadPart * 1000000) / freq.QuadPart;
	u64 result = 0;
	return result;
}

////////////////////////////////////////////////////////////////////////

void AndroidDevice::CreateWindowContext()
{
	if (!m_GLContextManager->IsContextCreated())
	{
		bool success = m_GLContextManager->CreateContext(true);
		m_driver->Init();
	}
	else
	{
		m_GLContextManager->DestroyContext(false);
		bool success = m_GLContextManager->CreateContext(false);
	}
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
	}
	break;
	default:
		break;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////