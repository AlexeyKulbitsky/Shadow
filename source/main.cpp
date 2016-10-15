#include "Shadow.h"
int main()
{
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