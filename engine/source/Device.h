#ifndef SHADOW_DEVICE_INCLUDE
#define SHADOW_DEVICE_INCLUDE

#include "CreationParameters.h"


namespace sh
{
	class Application;

	class SHADOW_API Device
	{
	public:
		Device();
		explicit Device(const CreationParameters &parameters);	
		virtual ~Device();

		virtual void Init();
		virtual void Run() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual uint64_t GetTime() = 0;

		void SetApplication(Application* application) { m_application = application; }
        
		static void SetInstance(Device* instance) { s_instance = instance; }
		static Device* GetInstance() { return s_instance; }
		static void Destroy();
		void* GetWinId() { return m_creationParameters.WinId; }
		void SetWinId(void* window) { m_creationParameters.WinId = window; }

	protected:
		CreationParameters m_creationParameters;
		Application* m_application = nullptr;

        uint64_t m_startTimePoint = 0U;
        uint64_t m_lastFrameTimePoint = 0U;
	private:
		static Device* s_instance;
	};
}

#endif
