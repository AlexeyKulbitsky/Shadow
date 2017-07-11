#include "Application.h"
#include "Shadow.h"

namespace sh
{
	Application::Application(const CreationParameters& params)
	{
		sh::Device* device = sh::CreateDevice(params);
		device->SetApplication(this);
	}

	void Application::Init()
	{
		Device::GetInstance()->Init();
	}

	void Application::Destroy()
	{
		Device::Destroy();
	}

	void Application::Run()
	{
		Device::GetInstance()->Run();
	}

} // sh