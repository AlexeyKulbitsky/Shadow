#include "TestApplication.h"

using namespace sh;


TestApp::TestApp()
{
}

TestApp::~TestApp()
{
}

void TestApp::Init()
{
	Application::Init();

	Device* device = Device::GetInstance();

	auto sceneMgr = device->GetSceneManager();
	sceneMgr->LoadScene("test_scene.xml");
	scene::Camera* camera = sceneMgr->GetCamera();
	camera->SetPosition(camera->GetPosition() + math::Vector3f(0.0f, 0.0f, 20.0f));
}

void TestApp::Destroy()
{
	Application::Destroy();
}

void TestApp::Update(u64 delta)
{
	const float deltaMilisec = static_cast<float>(delta) / 1000.0f;
	const float fps = 1000.0f / deltaMilisec;

	printf("FPS: %.2f\r", fps);

	Device* device = Device::GetInstance();

	InputManager* inputManager = device->GetInputManager();
	scene::SceneManager* sceneMgr = device->GetSceneManager();
	scene::Camera* camera = sceneMgr->GetCamera();

	video::Driver* driver = device->GetDriver();

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
}
