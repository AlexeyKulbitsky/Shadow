#include "App.h"
#include <iostream>

App::App()
{
}

App::~App()
{
}

void App::Init()
{
    Application::Init();
}

void App::Destroy()
{
    Application::Destroy();
}

void App::Update(uint64_t delta)
{
    std::cout << "Updating the application" << std::endl;
}
