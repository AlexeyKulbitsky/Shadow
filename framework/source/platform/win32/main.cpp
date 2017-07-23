#include "../../TestApplication.h"
int main()
{
	sh::Device *device = sh::CreateDevice();
    TestApp* application = new TestApp();
    device->SetApplication(application);
    device->Init();
    device->Run();
    device->Destroy();
    delete application;
	return 0;
}