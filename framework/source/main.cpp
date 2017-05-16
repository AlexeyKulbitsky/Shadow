#include <Shadow.h>

using namespace sh;
using namespace math;

int main()
{
	sh::CreationParameters params;
	params.width = 640;
	params.height = 480;
	//params.driverType = sh::video::DriverType::OPENGL_ES_2_0;
	params.driverType = sh::video::DriverType::VULKAN;
	//params.driverType = sh::video::DriverType::DIRECTX_11;
	sh::Device* device = sh::CreateDevice(params);

	sh::io::FileSystem* fs = device->GetFileSystem();
	fs->AddFolder(sh::String("../../../data"));
	fs->AddFolder(sh::String("../../../libs"));
	fs->Init();

	device->Init();

	sh::video::Driver* driver = device->GetDriver();
	//driver->SetClearColor(sh::math::Vector4f(0.5f, 0.5f, 0.5f, 1.0f));

	auto sceneMgr = device->GetSceneManager();
	sceneMgr->LoadScene("test_scene.xml");

	while (device->Run())
	{
		driver->BeginRendering();

		sceneMgr->Update();

		driver->EndRendering();
	}

	return 0;
}



