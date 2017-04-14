#include "Device.h"

namespace sh
{
	Device* Device::s_instance = nullptr;

	Device::Device()
	{
		m_inputManager.reset(new InputManager());
		m_resourceManager.reset(new ResourceManager());
	}

	Device::Device(const CreationParameters &parameters)
		: m_creationParameters(parameters)
	{
		m_fileSystem = new io::FileSystem();

		m_inputManager.reset(new InputManager());
		m_resourceManager.reset(new ResourceManager());
		m_resourceManager->Init();
	}

	Device::~Device()
	{
		int a = 0;
		a++;
	}

	void Device::SetWindow(void* window)
	{
		m_creationParameters.WinId = window;
	}

	void Device::Destroy()
	{
		delete s_instance;
		s_instance = nullptr;
	}
}