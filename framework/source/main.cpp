#include <Shadow.h>

using namespace sh;
using namespace math;

int main()
{
	sh::CreationParameters params;
	params.width = 640;
	params.height = 480;
	params.driverType = sh::video::DriverType::OPENGL_ES_2_0;
	//params.driverType = sh::video::DriverType::VULKAN;
	//params.driverType = sh::video::DriverType::DIRECTX_11;
	sh::Device* device = sh::CreateDevice(params);

	sh::io::FileSystem* fs = device->GetFileSystem();
	fs->AddFolder(sh::String("../../../data"));
	fs->AddFolder(sh::String("../../../libs"));
	fs->Init();

	device->Init();

	sh::video::Driver* driver = device->GetDriver();

	auto sceneMgr = device->GetSceneManager();
	sceneMgr->LoadScene("test_scene.xml");

	sh::InputManager* inputManager = device->GetInputManager();
	sh::scene::Camera* camera = sceneMgr->GetCamera();

	while (device->Run())
	{
		if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_W))
		{
			camera->SetPosition(camera->GetPosition() + camera->GetFrontVector() * 0.05f);
		}
		if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_S))
		{
			camera->SetPosition(camera->GetPosition() - camera->GetFrontVector() * 0.05f);
		}
		if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_D))
		{
			camera->SetPosition(camera->GetPosition() + camera->GetRightVector() * 0.05f);
		}
		if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_A))
		{
			camera->SetPosition(camera->GetPosition() - camera->GetRightVector() * 0.05f);
		}

		sh::math::Quaternionf rot;
		if (inputManager->IsKeyPressed(sh::KeyCode::KEY_UP))
		{
			sh::math::Quaternionf r;
			r.SetFromAxisAngle(camera->GetRightVector(), -0.01f);
			camera->SetRotation(r * camera->GetRotation());
		}
		if (inputManager->IsKeyPressed(sh::KeyCode::KEY_DOWN))
		{
			sh::math::Quaternionf r;
			r.SetFromAxisAngle(camera->GetRightVector(), 0.01f);
			camera->SetRotation(r * camera->GetRotation());
		}
		if (inputManager->IsKeyPressed(sh::KeyCode::KEY_RIGHT))
		{
			sh::math::Quaternionf r;
			r.SetFromAxisAngle(camera->GetUpVector(), -0.01f);
			camera->SetRotation(r * camera->GetRotation());
		}
		if (inputManager->IsKeyPressed(sh::KeyCode::KEY_LEFT))
		{
			sh::math::Quaternionf r;
			r.SetFromAxisAngle(camera->GetUpVector(), 0.01f);
			camera->SetRotation(r * camera->GetRotation());
		}
		driver->BeginRendering();

		sceneMgr->Update();

		driver->EndRendering();
	}

	return 0;
}



