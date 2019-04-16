#include "video/GL/GLContext/EGLContextManager.h"
#include <android/native_window.h>

namespace sh
{

namespace video
{

	EGLContextManager::EGLContextManager()
	{
	}

	EGLContextManager::~EGLContextManager()
	{

	}

	bool EGLContextManager::CreateContext(void* winId)
	{
		if (!m_isContextCreated)
		{
			m_eglNativeWindow = static_cast<EGLNativeWindowType>(winId);
			CreateContextInternal(true);
		}
		else
		{
			DestroyContextInternal(false);
			m_eglNativeWindow = static_cast<EGLNativeWindowType>(winId);
			CreateContextInternal(false);
		}

		return true;
	}

	bool EGLContextManager::DestroyContext()
	{
		return true;
	}

	void EGLContextManager::SwapBuffers()
	{
		int res = eglSwapBuffers(m_eglDisplay, m_eglSurface);
		if (res == EGL_FALSE)
		{
			return;
		}
		else if (res == EGL_BAD_DISPLAY)
		{
			return;
		}
		else if (res == EGL_NOT_INITIALIZED)
		{
			return;
		}
		else if (res == EGL_BAD_SURFACE)
		{
			return;
		}
		else if (res == EGL_CONTEXT_LOST)
		{
			return;
		}

	}

	bool EGLContextManager::IsContextCreated()
	{
		return m_isContextCreated;
	}

	bool EGLContextManager::CreateContextInternal(bool createDisplay)
	{
		EGLint attribList[] =
		{
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, m_glesContextVersion,
			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_ALPHA_SIZE, 8,
			EGL_DEPTH_SIZE, 16,
			//EGL_STENCIL_SIZE,   8,
			//EGL_SAMPLE_BUFFERS, 1,
			//EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			//EGL_RENDERABLE_TYPE, glesContextVersion,
			EGL_NONE
		};

		if (m_isContextCreated)
		{
			return false;
		}
		if (createDisplay) 
		{
			m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			if (m_eglDisplay == EGL_NO_DISPLAY)
			{
				return false;
			}
			if (!eglInitialize(m_eglDisplay, 0, 0)) 
			{
				return false;
			}
			EGLint mathConfigNum;
			if (!eglChooseConfig(m_eglDisplay, attribList, &m_config, 1, &mathConfigNum))
			{
				return false;
			}
	#ifdef ANDROID
			//new stuff
			EGLint format;
			if (!eglGetConfigAttrib(m_eglDisplay, m_config, EGL_NATIVE_VISUAL_ID, &format))
			{
				return false;
			}
			ANativeWindow_setBuffersGeometry(m_eglNativeWindow, 0, 0, format);
	#endif // ANDROID
		}
		//create surface
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_config, m_eglNativeWindow, NULL);
		if (m_eglSurface == EGL_NO_SURFACE)
		{
			return false;
		}
		EGLint contextAttribs[] {
			EGL_CONTEXT_CLIENT_VERSION, 2,
				EGL_NONE
		};
		//create context
		if (createDisplay) 
		{
			m_eglContext = eglCreateContext(m_eglDisplay, m_config, EGL_NO_CONTEXT, contextAttribs);

			if (m_eglContext == EGL_NO_CONTEXT)
			{
				return GL_FALSE;
			}
		}

		// Make the context current
		if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext))
		{
			EGLint error = eglGetError();
			return GL_FALSE;
		}
		m_isContextCreated = true;
		return m_isContextCreated;
	}

	bool EGLContextManager::DestroyContextInternal(bool destroyDisplay)
	{
		if (m_eglDisplay)
		{
			eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

			if (destroyDisplay)
			{
				if (m_eglContext != EGL_NO_CONTEXT)
				{
					eglDestroyContext(m_eglDisplay, m_eglContext);
					m_eglContext = EGL_NO_CONTEXT;
				}
			}

			if (m_eglSurface != EGL_NO_SURFACE)
			{
				eglDestroySurface(m_eglDisplay, m_eglSurface);
				m_eglSurface = EGL_NO_SURFACE;
			}
			if (destroyDisplay)
			{
				eglTerminate(m_eglDisplay);
				m_eglDisplay = EGL_NO_DISPLAY;
			}
		}
		m_isContextCreated = false;
		return true;
	}



	EGLint EGLContextManager::GetContextRenderableType(EGLDisplay eglDisplay)
	{
	#ifdef EGL_KHR_create_context
		const char *extensions = eglQueryString(eglDisplay, EGL_EXTENSIONS);

		// check whether EGL_KHR_create_context is in the extension string
		if (extensions != NULL && strstr(extensions, "EGL_KHR_create_context"))
		{
			// extension is supported
			return EGL_OPENGL_ES3_BIT_KHR;
		}
	#endif
		// extension is not supported
		return EGL_OPENGL_ES2_BIT;
	}

} // video

} // sh


