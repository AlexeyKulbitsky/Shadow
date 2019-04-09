#include "App.h"


int main()
{
    sh::Device *device = sh::CreateDevice();
    App* application = new App();
    device->SetApplication(application);
    device->Init();
    device->Run();
    device->Destroy();
    delete application;

    return 0;
}