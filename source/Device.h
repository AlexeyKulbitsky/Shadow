#ifndef SHADOW_DEVICE_INCLUDE
#define SHADOW_DEVICE_INCLUDE

#include "CreationParameters.h"
#include "Globals.h"

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

	class Device
	{
	public:
		Device();
		Device(const CreationParameters &parameters);	
		virtual ~Device();

		virtual bool Run() = 0;
		virtual void OnEvent(const Event& e) = 0;
		virtual u64 GetTime() = 0;

		virtual SHADOW_API video::Driver* SH_CALLCONV GetDriver() { return m_driver; }
		void SetSceneManager(scene::SceneManager* manager) { m_sceneManager = manager; }
		scene::SceneManager* GetSceneManager() { return m_sceneManager; }

		static void SetInstance(Device* instance) { m_instance = instance; }
		static Device* GetInstance() { return m_instance; }

	protected:
		CreationParameters m_creationParameters;
		video::GLContextManager *m_GLContextManager = nullptr;
		video::Driver *m_driver = nullptr;
		scene::SceneManager* m_sceneManager = nullptr;
		bool m_needsToClose = false;

	private:
		static Device* m_instance;
	};
}

#endif
