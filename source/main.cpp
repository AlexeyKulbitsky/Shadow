#include "Shadow.h"

int main()
{
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	sh::Device* device = sh::CreateDevice();
	if (device)
	{

		sh::video::Driver* driver = device->GetDriver();
		while (true)
		{
			driver->BeginRendering();

			driver->EndRendering();
		}
	}
	return 0;
}