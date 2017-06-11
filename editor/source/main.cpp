#include "MainWindow.h"

#include <Shadow.h>

#include <jobs/Test.h>

int main()
{
	//TestJobs();
	

	sh::CreationParameters params;
	params.width = 800;
	params.height = 400;
	params.driverType = sh::video::DriverType::OPENGL_ES_2_0;
	//params.driverType = sh::video::DriverType::VULKAN;
	//params.driverType = sh::video::DriverType::DIRECTX_11;
	sh::Device* device =  sh::CreateDevice(params);

	sh::io::FileSystem* fs = device->GetFileSystem();
	fs->AddFolder(sh::String("../../../data"));
	fs->AddFolder(sh::String("../../../libs"));
	fs->Init();

	device->Init();

	sh::video::Driver* driver = device->GetDriver();

	sh::gui::GuiManager::CreateInstance();

	sh::gui::GuiManager::GetInstance()->Init();

	MainWindow mainWindow;

	if (device)
	{
		while (device->Run())
		{
			mainWindow.Update();
		}
	}
	return 0;
}