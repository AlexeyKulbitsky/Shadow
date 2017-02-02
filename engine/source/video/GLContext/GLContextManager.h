#ifndef SHADOW_GLCONTEXTMANAGER_INCLUDE
#define SHADOW_GLCONTEXTMANAGER_INCLUDE

#include "../../CreationParameters.h"

namespace sh
{
	namespace video
	{
		class GLContextManager
		{
		public:
			//GLContextManager(){}
			//~GLContextManager(){}

			virtual bool InitContext(const CreationParameters &parameters) = 0;
			virtual bool AttachWindow(void* window) = 0;
			virtual bool CreateDisplay() = 0;
			virtual bool DestroyDisplay() = 0;
			virtual bool CreateContext(bool createDisplay) = 0;
			virtual bool DestroyContext(bool destroyDisplay) = 0;
			virtual bool SwapBuffers() = 0;
		};
	}
}

#endif
