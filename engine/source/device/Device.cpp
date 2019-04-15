#include "device/Device.h"

namespace sh
{
	Device* Device::s_instance = nullptr;

	Device::Device()
	{
	}

	Device::Device(const CreationParameters &parameters)
		: m_creationParameters(parameters)
	{
	}

	Device::~Device()
	{
	}

	void Device::Init()
	{
	}


	void Device::Destroy()
	{
		delete s_instance;
		s_instance = nullptr;
	}

    void* Device::GetWinId() 
    { 
        return m_creationParameters.WinId; 
    }

    void Device::SetWinId(void* window) 
    { 
        m_creationParameters.WinId = window; 
    }
}
