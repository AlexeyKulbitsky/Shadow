#ifndef SHADOW_GLCONTEXTMANAGER_INCLUDE
#define SHADOW_GLCONTEXTMANAGER_INCLUDE

#include "../../../CreationParameters.h"
#include "../GLCommon.h"

namespace sh
{
	namespace video
	{
		class GLContextManager
		{
		public:
			virtual bool CreateContext(void* winId) { return false; }
			virtual bool DestroyContext() = 0;
			virtual void SwapBuffers() = 0;
			virtual bool IsContextCreated() = 0;
		};
	}
}

#endif
