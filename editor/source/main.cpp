#include "MainWindow.h"

int main(int argc, const char * argv[])
{
	sh::Device *device = sh::CreateDevice();
	MainWindow* application = new MainWindow();
	device->SetApplication(application);
	device->Init();
	device->Run();
	device->Destroy();
	delete application;
	return 0;
}
