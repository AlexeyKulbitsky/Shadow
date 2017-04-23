#include "Device.h"

#include <functional>
using namespace std::placeholders;

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
		//m_fileSystem = new io::FileSystem();
		io::FileSystem::CreateInstance();
		m_fileSystem = io::FileSystem::GetInstance();

		m_inputManager.reset(new InputManager());

		mouseEvent.Connect(std::bind(&InputManager::OnMouseEvent, m_inputManager.get(), _1, _2, _3, _4));
		keyboardEvent.Connect(std::bind(&InputManager::OnKeyboardEvent, m_inputManager.get(), _1, _2));

		m_resourceManager.reset(new ResourceManager());
		m_resourceManager->Init();
	}

	Device::~Device()
	{
		int a = 0;
		a++;

		//io::FileSystem::DestroyInstance();
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