#include <Shadow.h>

using namespace sh;
using namespace math;

int main()
{
	sh::CreationParameters params;
	params.width = 800;
	params.height = 600;
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

	const float moveSpeed = 1.0f;
	const float mouseSens = -0.01f;
	while (device->Run())
	{
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

		printf("Time: %f\r", static_cast<float>(device->GetTime()));
		//const auto forward = camera->GetFrontVector();
		//printf("x:%f y:%f z:%f\r", forward.x, forward.y, forward.z);
	}

	return 0;
}



