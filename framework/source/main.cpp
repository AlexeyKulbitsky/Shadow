#if 0
#include "TestApplication.h"

int main()
{
	sh::Device* device = sh::CreateDevice();
	sh::CreationParameters params;
	params.width = 800;
	params.height = 600;
	params.driverType = sh::video::DriverType::OPENGL_ES_2_0;
	//params.driverType = sh::video::DriverType::VULKAN;
	//params.driverType = sh::video::DriverType::DIRECTX_11;

	TestApp app(params);
	app.Init();
	app.Run();
	app.Destroy();

	return 0;
}

#endif



