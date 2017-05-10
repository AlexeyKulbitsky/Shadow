#include "EGLContextManager.h"

#ifdef SHADOW_ANDROID
#include <android/native_window.h>
#endif

using namespace sh;
using namespace video;

EGLContextManager::EGLContextManager()
{

}

EGLContextManager::~EGLContextManager()
{

}

bool EGLContextManager::InitContext(const CreationParameters &parameters)
{
	switch (parameters.driverType)
	{
	case DriverType::OPENGL_ES_2_0:
		m_glesApiVersion = 2;
		m_glesContextVersion = EGL_OPENGL_ES2_BIT;
		break;
	case DriverType::OPENGL_ES_3_0:
		m_glesApiVersion = 3;
		m_glesContextVersion = EGL_OPENGL_ES3_BIT_KHR;
		break;
	default:
		break;
	}

	if (!AttachWindow(parameters.WinId))
		return false;

//	if (!CreateContext(true))
//		return false;

//#if 0

#ifndef SHADOW_APPLE
	EGLint majorVersion;
	EGLint minorVersion;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, m_glesApiVersion, EGL_NONE };

#ifdef SHADOW_ANDROID
	// For Android, get the width/height from the window rather than what the
	// application requested.
	m_width = ANativeWindow_getWidth(m_eglNativeWindow);
	m_height = ANativeWindow_getHeight(m_eglNativeWindow);
#else
	m_width = parameters.width;
	m_height = parameters.height;
#endif

#if defined SHADOW_WINDOWS
	m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#else
	m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#endif


	if (m_eglDisplay == EGL_NO_DISPLAY)
	{
		return GL_FALSE;
	}

	// Initialize EGL
	if (!eglInitialize(m_eglDisplay, &majorVersion, &minorVersion))
	{
		return GL_FALSE;
	}


	u32 supportedContext = GetContextRenderableType(m_eglDisplay);

	if (m_glesContextVersion == EGL_OPENGL_ES3_BIT_KHR && supportedContext != m_glesContextVersion)
	{
		// Error: this context (OpenGL ES 3.0) is not supported
		exit(0);
	}

	{
		EGLint numConfigs = 0;
#if 0
		EGLint attribList[] =
		{
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, m_glesContextVersion,
			EGL_RED_SIZE,       8,
			EGL_GREEN_SIZE,     8,
			EGL_BLUE_SIZE,      8,
			EGL_ALPHA_SIZE,     8,
			EGL_DEPTH_SIZE,		8,
			EGL_STENCIL_SIZE,   8,
			//EGL_SAMPLE_BUFFERS, 1,
			//EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			//EGL_RENDERABLE_TYPE, glesContextVersion,
			EGL_NONE
		};
#endif
		EGLint attribList[] =
		{
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, m_glesContextVersion,
			EGL_RED_SIZE,       8,
			EGL_GREEN_SIZE,     8,
			EGL_BLUE_SIZE,      8,
			EGL_ALPHA_SIZE,     8,
			EGL_DEPTH_SIZE,		16,
			//EGL_STENCIL_SIZE,   8,
			//EGL_SAMPLE_BUFFERS, 1,
			//EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			//EGL_RENDERABLE_TYPE, glesContextVersion,
			EGL_NONE
		};

		// Choose config
		if (!eglChooseConfig(m_eglDisplay, attribList, &m_config, 1, &numConfigs))
		{
			return GL_FALSE;
		}

		if (numConfigs < 1)
		{
			return GL_FALSE;
		}
	}

#ifdef SHADOW_ANDROID
	// For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
	{
		EGLint format = 0;
		if (!eglGetConfigAttrib(m_eglDisplay, m_config, EGL_NATIVE_VISUAL_ID, &format))
		{
			return GL_FALSE;
		}
		ANativeWindow_setBuffersGeometry(m_eglNativeWindow, 0, 0, format);
	}
#endif // ANDROID

	// Create a surface
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_config, m_eglNativeWindow, NULL);

	if (m_eglSurface == EGL_NO_SURFACE)
	{
		return GL_FALSE;
	}

	// Create a GL context
	m_eglContext = eglCreateContext(m_eglDisplay, m_config, EGL_NO_CONTEXT, contextAttribs);

	if (m_eglContext == EGL_NO_CONTEXT)
	{
		return GL_FALSE;
	}

	// Make the context current
	if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext))
	{
		return GL_FALSE;
	}

#endif

//#endif
	return true;
}

bool EGLContextManager::AttachWindow(void* window)
{
	m_eglNativeWindow = static_cast<EGLNativeWindowType>(window);
	if (m_eglNativeWindow)
		return true;

	return false;
}

bool EGLContextManager::CreateDisplay()
{
	EGLint majorVersion;
	EGLint minorVersion;


#if defined SHADOW_WINDOWS
	m_eglDisplay = eglGetDisplay(m_eglNativeDisplay);
#else
	m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#endif


	if (m_eglDisplay == EGL_NO_DISPLAY)
	{
		return GL_FALSE;
	}

	// Initialize EGL
	if (!eglInitialize(m_eglDisplay, &majorVersion, &minorVersion))
	{
		return GL_FALSE;
	}


	u32 supportedContext = GetContextRenderableType(m_eglDisplay);

	//if (glesContextVersion == EGL_OPENGL_ES3_BIT_KHR && supportedContext != glesContextVersion)
	//{
		// Error: this context (OpenGL ES 3.0) is not supported
	//	exit(0);
	//}

	{
		EGLint numConfigs = 0;
		EGLint attribList[] =
		{
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, m_glesContextVersion,
			EGL_RED_SIZE,       8,
			EGL_GREEN_SIZE,     8,
			EGL_BLUE_SIZE,      8,
			EGL_ALPHA_SIZE,     8,
			EGL_DEPTH_SIZE,		24,
			EGL_STENCIL_SIZE,   8,
			//EGL_SAMPLE_BUFFERS, 0,
			//EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			//EGL_RENDERABLE_TYPE, glesContextVersion,
			EGL_NONE
		};

		// Choose config
		if (!eglChooseConfig(m_eglDisplay, attribList, &m_config, 1, &numConfigs))
		{
			return GL_FALSE;
		}

		if (numConfigs < 1)
		{
			return GL_FALSE;
		}
	}

#ifdef SHADOW_ANDROID
	// For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
	{
		EGLint format = 0;
		if (!eglGetConfigAttrib(m_eglDisplay, m_config, EGL_NATIVE_VISUAL_ID, &format))
		{
			return GL_FALSE;
		}
		ANativeWindow_setBuffersGeometry(m_eglNativeWindow, 0, 0, format);
	}
#endif // ANDROID
	return GL_TRUE;
}

//////////////////////////////////////////////////////////////////////////////

bool EGLContextManager::DestroyDisplay()
{
	return false;
}

//////////////////////////////////////////////////////////////////////////////

bool EGLContextManager::CreateContext(bool createDisplay)
{
	if (createDisplay)
	{
		if (!CreateDisplay())
		{
			return false;
		}
	}

	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, m_glesApiVersion, EGL_NONE };

	// Create a surface
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_config, m_eglNativeWindow, NULL);

	if (m_eglSurface == EGL_NO_SURFACE)
	{
		return GL_FALSE;
	}

	// Create a GL context
	m_eglContext = eglCreateContext(m_eglDisplay, m_config, EGL_NO_CONTEXT, contextAttribs);

	if (m_eglContext == EGL_NO_CONTEXT)
	{
		return GL_FALSE;
	}

	// Make the context current
	if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext))
	{
		return GL_FALSE;
	}
}

bool EGLContextManager::DestroyContext(bool destroyDisplay)
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
	return true;
}

bool EGLContextManager::SwapBuffers()
{
	int res = eglSwapBuffers(m_eglDisplay, m_eglSurface);
	if (res == EGL_FALSE)
	{
		return false;
	}
	else if (res == EGL_BAD_DISPLAY)
	{
		return false;
	}
	else if (res == EGL_NOT_INITIALIZED)
	{
		return false;
	}
	else if (res == EGL_BAD_SURFACE)
	{
		return false;
	}
	else if (res == EGL_CONTEXT_LOST)
	{
		return false;
	}

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