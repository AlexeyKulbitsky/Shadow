#include "Connection.h"

// Components
#include "components/TestComponent.h"

// Scripts
#include "components/ScriptA.h"
#include "components/ScriptB.h"

// Game itself
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

	// Register scripts;
	ScriptA::RegisterObject();
	ScriptB::RegisterObject();
}