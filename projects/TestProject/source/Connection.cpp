#include "Connection.h"

#include "components/TestComponent.h"
#include "Game.h"

static sh::Application* application = nullptr;

__declspec(dllexport) sh::Application* CreateGameModule()
{
	application = new Game();
	return application;
}

__declspec(dllexport) void DestroyGameModule()
{
	if (application)
		delete application;
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