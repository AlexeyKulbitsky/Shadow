#include "Connection.h"

#include "components/TestComponent.h"
#include "Game.h"

__declspec(dllexport) sh::Application* GetGameModule()
{
	return new Game();
}

__declspec(dllexport) void SetDevice(sh::Device* device)
{
	if (!device)
		return;

	sh::Device::SetInstance(device);
	sh::ObjectFactory::SetInstance(device->GetObjectFactory());

	// Register components
	TestComponent::RegisterObject();
}