#include "Device.h"

namespace sh
{
	Device* Device::m_instance = nullptr;

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
}