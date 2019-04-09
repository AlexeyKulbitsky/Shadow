#ifndef SHADOW_SYSTEM_INCLUDE
#define SHADOW_SYSTEM_INCLUDE

#include "Globals.h"
#include "CreationParameters.h"
//#include "InputManager.h"
//#include "io/FileSystem.h"
//#include "resources/ResourceManager.h"

namespace sh
{
	namespace video
	{
		class Driver;
	}

	namespace scene
	{
		class SceneManager;
	}

	class Application;
	class ObjectFactory;

	class System
	{
	/*public:
		static video::Driver* GetDriver() { return s_driver; }
		static scene::SceneManager* GetSceneManager() { return s_sceneManager; }
		static InputManager* GetInputManager() { return s_inputManager; }
		static io::FileSystem* GetFileSystem() { return s_fileSystem; }
		static Application* GetApplication() { return s_application; }
		static ObjectFactory* GetObjectFactory() { return s_objectFactory; }

	private:
		static video::Driver* s_driver;
		static scene::SceneManager* s_sceneManager;
		static InputManager* s_inputManager;
		static io::FileSystem* s_fileSystem;
		static Application* s_application;
		static ObjectFactory* s_objectFactory;*/
	};

} // sh

#endif
