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
	device->Init();

	sh::io::FileSystem* fs = device->GetFileSystem();
	fs->AddFolder(sh::String("../../../data"));
	fs->Init();

	sh::video::Driver* driver = device->GetDriver();
	//driver->SetViewport(0U, 0U, 640U, 480U);
	driver->SetViewport(0U, 0U, 300U, 300U);
	driver->SetClearColor(sh::math::Vector4f(0.5f, 0.5f, 0.5f, 1.0f));

	sh::scene::SceneManager* sceneMgr = new sh::scene::SceneManager();
	sh::ComponentsFactory* factory = new sh::ComponentsFactory();
	sceneMgr->SetComponentsFactory(factory);
	device->SetSceneManager(sceneMgr);

	sh::scene::Camera* camera = new sh::scene::Camera();
	camera->SetProjection(3.1415926535f / 3.0f, params.width, params.height, 0.1f, 1000.0f);
	camera->SetPosition(sh::math::Vector3f(0.0f));
	sceneMgr->SetCamera(camera);

	sceneMgr->LoadScene("test_scene.xml");

	if (device)
	{
		while (device->Run())
		{
			driver->BeginRendering();

			driver->EndRendering();
		}
	}

	return 0;
}



