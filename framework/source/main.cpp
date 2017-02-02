#include <Shadow.h>

int main()
{
	sh::Device* device = sh::CreateDevice();
	sh::video::Driver* driver = device->GetDriver();
	driver->SetClearColor(sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f));
	
	sh::scene::SceneManager* sceneMgr = new sh::scene::SceneManager();
	device->SetSceneManager(sceneMgr);
	
	if (device)
	{
		while (device->Run())
		{
			driver->BeginRendering();

			driver->EndRendering();
		}
	}
	return 0;
}



