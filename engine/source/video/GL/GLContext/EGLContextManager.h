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

			virtual bool InitContext(const CreationParameters &parameters) override;
			virtual bool AttachWindow(void* window) override;
			virtual bool CreateDisplay() override;
			virtual bool DestroyDisplay() override;
			virtual bool CreateContext(bool createDisplay) override;
			virtual bool DestroyContext(bool destroyDisplay) override;
			virtual bool SwapBuffers() override;
			virtual bool IsContextCreated() override { return false; }// { return m_isContextCreated; }

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

		};
	}
}

#endif


#endif
