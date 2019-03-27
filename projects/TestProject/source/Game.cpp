#include "Game.h"
#include "Connection.h"

#include <iostream>

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{
	sh::Application::Init();
    
    InitGameModule();
    
    sh::io::FileSystem::GetInstance()->AddFolder("../../assets");

	sh::Device* device = sh::Device::GetInstance();

	auto sceneMgr = device->GetSceneManager();
	sceneMgr->LoadScene("test_scene.xml");
	sh::scene::CameraPtr camera = sceneMgr->GetCamera();
	camera->SetPosition(camera->GetPosition() + sh::math::Vector3(0.0f, 0.0f, 20.0f));

    sceneMgr->SetSystemsState(sh::SystemState::Running);
}

void Game::Destroy()
{
	sh::Application::Destroy();
}

void Game::Update(sh::u64 delta)
{
#if 0
	const float deltaMilisec = static_cast<float>(delta) / 1000.0f;
	const float fps = 1000.0f / deltaMilisec;

	printf("FPS: %.2f\r", fps);

	sh::Device* device = sh::Device::GetInstance();

	sh::InputManager* inputManager = device->GetInputManager();
	sh::scene::SceneManager* sceneMgr = device->GetSceneManager();
	sh::scene::Camera* camera = sceneMgr->GetCamera();

	sh::video::Driver* driver = device->GetDriver();

	const float moveSpeed = 1.0f;
	const float mouseSens = -0.01f;
	if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_W))
	{
		camera->SetPosition(camera->GetPosition() + camera->GetFrontVector() * moveSpeed);
	}
	if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_S))
	{
		camera->SetPosition(camera->GetPosition() - camera->GetFrontVector() * moveSpeed);
	}
	if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_D))
	{
		camera->SetPosition(camera->GetPosition() + camera->GetRightVector() * moveSpeed);
	}
	if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_A))
	{
		camera->SetPosition(camera->GetPosition() - camera->GetRightVector() * moveSpeed);
	}

	if (inputManager->IsMouseButtonPressed(sh::MouseCode::ButtonLeft))
	{
		const auto delta = inputManager->GetMousePositionCurrent() - inputManager->GetMousePositionOld();

		sh::math::Quaternionf r1;
		r1.SetFromAxisAngle(camera->GetRightVector(), (float)delta.y * mouseSens);
		camera->SetRotation(r1 * camera->GetRotation());

		sh::math::Quaternionf r2;
		r2.SetFromAxisAngle(sceneMgr->GetUpVector(), (float)delta.x * mouseSens);
		camera->SetRotation(r2 * camera->GetRotation());

		inputManager->SetMousePositionOld(inputManager->GetMousePositionCurrent());
	}

	driver->BeginRendering();

	sceneMgr->Update();

	driver->EndRendering();
#endif

	sh::Device* device = sh::Device::GetInstance();
    sh::InputManager* inputManager = device->GetInputManager();
    sh::scene::SceneManager* sceneMgr = device->GetSceneManager();
    sh::scene::CameraPtr camera = sceneMgr->GetCamera();
    
    sh::video::Driver* driver = device->GetDriver();
    
    driver->BeginRendering();
    
    sceneMgr->Update();
    
    driver->EndRendering();
}
