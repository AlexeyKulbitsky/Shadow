#include "Device.h"

namespace sh
{
	Device* Device::m_instance = nullptr;

	Device::Device()
	{
		m_inputManager = new InputManager();
		m_resourceManager = new ResourceManager();
	}

	Device::Device(const CreationParameters &parameters)
		: m_creationParameters(parameters)
	{
		m_fileSystem = new io::FileSystem();
		m_fileSystem->AddFolder(String("../../../data"));
		m_fileSystem->Init();

		m_inputManager = new InputManager();
		m_resourceManager = new ResourceManager();
		m_resourceManager->Init();
	}

	Device::~Device()
	{
	}
}