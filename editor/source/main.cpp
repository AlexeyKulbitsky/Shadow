/*
#include <QtWidgets/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	
	MainWindow window;
	window.show();
	
	return a.exec();
}
*/

#include "MainWindow.h"

#include <Shadow.h>

int main()
{
	sh::CreationParameters params;
	params.width = 640;
	params.height = 480;
	//params.driverType = sh::video::DriverType::OPENGL_ES_2_0;
	params.driverType = sh::video::DriverType::VULKAN;
	//params.driverType = sh::video::DriverType::DIRECTX_11;
	sh::Device* device =  sh::CreateDevice(params);

	sh::video::Driver* driver = device->GetDriver();
	driver->SetClearColor(sh::math::Vector4f(0.7f, 0.7f, 0.7f, 1.0f));
	
	sh::scene::ModelLoader* modelLoader = new sh::scene::TinyObjModelLoader();
	sh::scene::ModelLoader::SetInstance(modelLoader);

	sh::scene::SceneManager* sceneMgr = new sh::scene::SceneManager();
	sh::ComponentsFactory* factory = new sh::ComponentsFactory();
	sceneMgr->SetComponentsFactory(factory);
	device->SetSceneManager(sceneMgr);

	sh::gui::GuiManager::CreateInstance();


	sh::io::FileSystem* fs = device->GetFileSystem();
	fs->AddFolder(sh::String("../../../../../data"));
	fs->AddFolder(sh::String("../../../../../libs"));
	fs->Init();


	sh::scene::Camera* camera = new sh::scene::Camera();
	camera->SetProjection(3.1415926535f / 4.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	camera->SetPosition(sh::math::Vector3f(0.0f));
	sceneMgr->SetCamera(camera);

	sh::gui::GuiManager::GetInstance()->Init();

	MainWindow mainWindow;

	if (device)
	{
		while (device->Run())
		{
			mainWindow.Update();

			driver->BeginRendering();

			sceneMgr->Update();
			
			sh::gui::GuiManager::GetInstance()->Render();

			driver->EndRendering();
		}
	}
	return 0;
}