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
			virtual bool SwapBuffers() = 0;
		};
	}
}

#endif
