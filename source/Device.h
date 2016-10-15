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
		Device() {}
		Device(const CreationParameters &parameters)
			: m_creationParameters(parameters)
		{
		}
		virtual ~Device() {}

		virtual SHADOW_API video::Driver* SH_CALLCONV GetDriver() { return m_driver; }

	protected:
		CreationParameters m_creationParameters;
		video::GLContextManager *m_GLContextManager = nullptr;
		video::Driver *m_driver = nullptr;
	};
}

#endif
