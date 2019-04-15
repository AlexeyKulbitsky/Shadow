#ifndef SHADOW_GL_DEBUG_INCLUDE
#define SHADOW_GL_DEBUG_INCLUDE

#include "video/GL/GLCommon.h"

namespace sh
{

namespace video
{
#define GL_CALL(x)									\
{													\
	(x);											\
	GLDebug::CheckGLError(__FILE__, __LINE__);		\
}

    class GLDebug
    {
    public:
        static void CheckGLError(const char* file, unsigned line);
    };

} // video

} // sh

#endif
