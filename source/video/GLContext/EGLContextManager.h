#ifndef SHADOW_EGLCONTEXTMANAGER_INCLUDE
#define SHADOW_EGLCONTEXTMANAGER_INCLUDE

#include "GLContextManager.h"

#include <stdlib.h>

#ifdef SHADOW_APPLE
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#if defined SHADOW_WINDOWS
//#pragma comment(lib, "libEGL.lib")
//#pragma comment(lib, "libGLESv2.lib")
#endif

namespace sh
{
	namespace video
	{
		class EGLContextManager : public GLContextManager
		{
		public:
			EGLContextManager();
			virtual ~EGLContextManager();

			virtual bool InitContext(const CreationParameters &parameters) override;
			virtual bool SwapBuffers() override;

		private:
			GLint       m_width;
			GLint       m_height;

#ifndef SHADOW_APPLE

			EGLNativeDisplayType	m_eglNativeDisplay;
			EGLNativeWindowType		m_eglNativeWindow;
			EGLDisplay				m_eglDisplay;
			EGLContext				m_eglContext;
			EGLSurface				m_eglSurface;
#endif
		};
	}
}

#endif
