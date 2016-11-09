#ifndef SHADOW_DEVICE_INCLUDE
#define SHADOW_DEVICE_INCLUDE

#include "Types.h"
#include "video/GLContext/GLContextManager.h"
#include "CreationParameters.h"

namespace sh
{
	namespace video
	{
		class GLContextManager;
		class Driver;
	}

	class Device
	{
	public:
		Device();
		Device(const CreationParameters &parameters);	
		virtual ~Device();

		virtual SHADOW_API video::Driver* SH_CALLCONV GetDriver() { return m_driver; }

		static void SetInstance(Device* instance) { m_instance = instance; }
		static Device* GetInstance() { return m_instance; }

	protected:
		CreationParameters m_creationParameters;
		video::GLContextManager *m_GLContextManager = nullptr;
		video::Driver *m_driver = nullptr;

	private:
		static Device* m_instance;
	};
}

#endif
