#include "Device.h"

#include "scene/ModelLoader/TinyObjModelLoader.h"
#include "scene/ModelLoader/AssimpModelLoader.h"

#include "scene/SceneManager.h"
#include "scene/Camera.h"
#include "entity/ComponentsFactory.h"

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
		// Model loader
		scene::ModelLoader::CreateInstance<scene::AssimpModelLoader>();
		//scene::ModelLoader::CreateInstance<scene::TinyObjModelLoader>();

		// Scene manager
		sh::scene::SceneManager* sceneMgr = new sh::scene::SceneManager();
		sh::ComponentsFactory* factory = new sh::ComponentsFactory();
		sceneMgr->SetComponentsFactory(factory);
		SetSceneManager(sceneMgr);

		// Camera
		sh::scene::Camera* camera = new sh::scene::Camera();
		camera->SetProjection(3.1415926535f / 3.0f, m_creationParameters.width, m_creationParameters.height, 0.1f, 1000.0f);
		camera->SetPosition(sh::math::Vector3f(0.0f));
		sceneMgr->SetCamera(camera);
	}


	void Device::Destroy()
	{
		delete s_instance;
		s_instance = nullptr;
	}
}