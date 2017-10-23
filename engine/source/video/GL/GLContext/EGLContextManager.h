#ifndef SHADOW_EGLCONTEXTMANAGER_INCLUDE
#define SHADOW_EGLCONTEXTMANAGER_INCLUDE

#if defined SHADOW_ANDROID


#include "GLContextManager.h"

#include <stdlib.h>

namespace sh
{
	namespace video
	{
		class EGLContextManager : public GLContextManager
		{
		public:
			EGLContextManager();
			virtual ~EGLContextManager();
			virtual bool CreateContext(void* winId) override;

			virtual void SwapBuffers() override;
			virtual bool IsContextCreated() override { return m_isContextCreated; }

		private:
			bool CreateContextInternal(bool createDisplay);
			bool DestroyContextInternal(bool destroyDisplay);
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

		};
	}
}

#endif


#endif
