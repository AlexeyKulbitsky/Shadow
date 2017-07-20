#include "Device.h"

#include "scene/ModelLoader/TinyObjModelLoader.h"
#include "scene/ModelLoader/AssimpModelLoader.h"

#include "scene/SceneManager.h"
#include "scene/Camera.h"
#include "entity/ComponentsFactory.h"

#include "gui/GuiManager.h"
#include "gui/SpriteManager.h"
#include "font/FontManager.h"

#include <functional>
using namespace std::placeholders;

namespace sh
{
	Device* Device::s_instance = nullptr;

	Device::Device()
	{
		m_inputManager.reset(new InputManager());

		mouseEvent.Connect(std::bind(&InputManager::OnMouseEvent, m_inputManager.get(), _1, _2, _3, _4));
		keyboardEvent.Connect(std::bind(&InputManager::OnKeyboardEvent, m_inputManager.get(), _1, _2));

		m_resourceManager.reset(new ResourceManager());
		m_resourceManager->Init();
	}

	Device::Device(const CreationParameters &parameters)
		: m_creationParameters(parameters)
	{
		m_inputManager.reset(new InputManager());

		mouseEvent.Connect(std::bind(&InputManager::OnMouseEvent, m_inputManager.get(), _1, _2, _3, _4));
		keyboardEvent.Connect(std::bind(&InputManager::OnKeyboardEvent, m_inputManager.get(), _1, _2));

		m_resourceManager.reset(new ResourceManager());
		m_resourceManager->Init();
	}

	Device::~Device()
	{
	}

	void Device::Init()
	{
		// File system
		m_fileSystem->Init();

		// Model loader
		//scene::ModelLoader::CreateInstance<scene::AssimpModelLoader>();
		scene::ModelLoader::CreateInstance<scene::TinyObjModelLoader>();

		// Scene manager
		sh::scene::SceneManager* sceneMgr = new sh::scene::SceneManager();
		sh::ComponentsFactory* factory = new sh::ComponentsFactory();
		sceneMgr->SetComponentsFactory(factory);
		m_sceneManager = sceneMgr;

		// Camera
		sh::scene::Camera* camera = new sh::scene::Camera();
		camera->SetProjection(3.1415926535f / 3.0f, 
			static_cast<f32>(m_creationParameters.width), 
			static_cast<f32>(m_creationParameters.height), 0.1f, 1000.0f);
		camera->SetPosition(sh::math::Vector3f(0.0f));
		sceneMgr->SetCamera(camera);

		windowResizeEvent.Connect(std::bind(&scene::SceneManager::OnWindowResized, sceneMgr,
			std::placeholders::_1, std::placeholders::_2));

		FontManager::CreateInstance();
		gui::SpriteManager::CreateInstance();
	}


	void Device::Destroy()
	{
		gui::SpriteManager::DestroyInstance();
		FontManager::DestroyInstance();

		io::FileSystem::DestroyInstance();

		delete s_instance;
		s_instance = nullptr;
	}
}