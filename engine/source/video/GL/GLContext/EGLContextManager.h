#ifndef SHADOW_EGLCONTEXTMANAGER_INCLUDE
#define SHADOW_EGLCONTEXTMANAGER_INCLUDE

#include "GLContextManager.h"

#include <stdlib.h>
#include <gl/glew.h>
#if 0


#ifdef SHADOW_APPLE
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#if defined SHADOW_WINDOWS
//#pragma comment(lib, "libEGL.lib")
//#pragma comment(lib, "libGLESv2.lib")
#endif

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
			virtual bool AttachWindow(void* window) override;
			virtual bool CreateDisplay() override;
			virtual bool DestroyDisplay() override;
			virtual bool CreateContext(bool createDisplay) override;
			virtual bool DestroyContext(bool destroyDisplay) override;
			virtual bool SwapBuffers() override;
			virtual bool IsContextCreated() override { return false; }// { return m_isContextCreated; }
#if 0
		private:
			EGLint GetContextRenderableType(EGLDisplay eglDisplay);

		private:
			GLint       m_width;
			GLint       m_height;
			s32			m_glesApiVersion = 0U;
			s32			m_glesContextVersion = 0U;
			EGLConfig	m_config;
			bool		m_displayEmpty = true;

			bool		m_isContextCreated = false;

#if !defined SHADOW_APPLE
			EGLNativeDisplayType	m_eglNativeDisplay;
			EGLNativeWindowType		m_eglNativeWindow;
			EGLDisplay				m_eglDisplay;
			EGLContext				m_eglContext;
			EGLSurface				m_eglSurface;
#endif

#endif
		};
	}
}

#endif
