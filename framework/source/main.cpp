#include <Shadow.h>
#include <scene\Camera.h>
#include <scene\SceneManager.h>
#include <scene\ModelLoader\TinyObjModelLoader.h>

int main()
{
	sh::Device* device = sh::CreateDevice();
	sh::video::Driver* driver = device->GetDriver();

	
	sh::scene::ModelLoader* modelLoader = new sh::scene::TinyObjModelLoader();
	sh::scene::ModelLoader::SetInstance(modelLoader);
	
	sh::scene::SceneManager* sceneMgr = new sh::scene::SceneManager();
	device->SetSceneManager(sceneMgr);

	
	sh::io::FileSystem* fs = device->GetFileSystem();
	sh::io::FileInfo finfo = fs->FindFile(sh::String("test_scene.xml"));
	if (finfo.name == "")
		return 0;
	
	sceneMgr->LoadScene(finfo.absolutePath.c_str());

	sh::scene::Camera* camera = new sh::scene::Camera();
	camera->SetProjection(3.1415926535f / 4.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	camera->SetPosition(sh::math::Vector3f(0.0f));
	sceneMgr->SetCamera(camera);

	
	if (device)
	{
		while (device->Run())
		{
			driver->BeginRendering();
			sceneMgr->Update();
			sceneMgr->Render();

			driver->EndRendering();
		}
	}
	return 0;
}


