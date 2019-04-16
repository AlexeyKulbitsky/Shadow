#ifndef SHADOW_EGLCONTEXTMANAGER_INCLUDE
#define SHADOW_EGLCONTEXTMANAGER_INCLUDE

#include "video/GL/GLContext/GLContextManager.h"

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
			bool CreateContext(void* winId) override final;
			bool DestroyContext() override final;
			void SwapBuffers() override final;
			bool IsContextCreated() override final;

		private:
			bool CreateContextInternal(bool createDisplay);
			bool DestroyContextInternal(bool destroyDisplay);
			EGLint GetContextRenderableType(EGLDisplay eglDisplay);

		private:
			int32_t m_glesApiVersion = 0U;
            int32_t m_glesContextVersion = 0U;
			EGLConfig m_config;

			bool m_isContextCreated = false;

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
