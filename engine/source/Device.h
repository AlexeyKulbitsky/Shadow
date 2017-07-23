#ifndef SHADOW_DEVICE_INCLUDE
#define SHADOW_DEVICE_INCLUDE

#include "Globals.h"
#include "CreationParameters.h"
#include "InputManager.h"
#include "io/FileSystem.h"
#include "resources/ResourceManager.h"


namespace sh
{
	namespace video
	{
		class GLContextManager;
		class Driver;
	}

	namespace scene
	{
		class SceneManager;
	}

	class Application;

	class Device
	{
	public:
		Device();
		explicit Device(const CreationParameters &parameters);	
		virtual ~Device();

		virtual void Init();
		virtual void Run() = 0;
		virtual void Update(f32 deltaTime) = 0;
		virtual u64 GetTime() = 0;

		void SetApplication(Application* application) { m_application = application; }

		video::Driver* GetDriver() { return m_driver; }
		scene::SceneManager* GetSceneManager() { return m_sceneManager; }
		InputManager* GetInputManager() { return m_inputManager.get(); }
		ResourceManager* GetResourceManager() { return m_resourceManager.get(); }
		io::FileSystem* GetFileSystem() { return m_fileSystem; }
		video::GLContextManager* GetContextManager() { return m_GLContextManager;}

		static void SetInstance(Device* instance) { s_instance = instance; }
		static Device* GetInstance() { return s_instance; }
		static void Destroy();
		void* GetWinId() { return m_creationParameters.WinId; }
		void SetWinId(void* window) { m_creationParameters.WinId = window; }

		Event<void, int, int, MouseEventType, MouseCode> mouseEvent;
		Event<void, int> mouseWheelEvent;
		Event<void, KeyboardEventType, KeyCode> keyboardEvent;
		Event<void, int, int> windowResizeEvent;
		Event<void, void*, int, int> sursafeChangedEvent;

	protected:
		CreationParameters m_creationParameters;
		video::GLContextManager *m_GLContextManager = nullptr;
		video::Driver *m_driver = nullptr;
		scene::SceneManager* m_sceneManager = nullptr;

		InputManagerUPtr m_inputManager;
		ResourceManagerUPtr m_resourceManager;

		io::FileSystem* m_fileSystem = nullptr;

		Application* m_application = nullptr;

		u64 m_startTimePoint = 0U;
		u64 m_lastFrameTimePoint = 0U;

	private:
		static Device* s_instance;
	};
}

#endif
