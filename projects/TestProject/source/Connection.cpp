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

__declspec(dllexport) void InitGameModule()
{
	// Register components
	TestComponent::RegisterObject();
}