#include "System.h"

namespace sh
{

	video::Driver* System::s_driver = nullptr;
	scene::SceneManager* System::s_sceneManager = nullptr;
	InputManager* System::s_inputManager;
	io::FileSystem* System::s_fileSystem = nullptr;
	Application* System::s_application = nullptr;
	ObjectFactory* System::s_objectFactory = nullptr;

} // sh