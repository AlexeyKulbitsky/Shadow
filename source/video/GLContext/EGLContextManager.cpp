#include "EGLContextManager.h"

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
	s32 glesApiVersion = 0U;
	s32 glesContextVersion = 0U;
	switch (parameters.driverType)
	{
	case DriverType::OPENGL_ES_2_0:
		glesApiVersion = 2;
		glesContextVersion = EGL_OPENGL_ES2_BIT;
		break;
	case DriverType::OPENGL_ES_3_0:
		glesApiVersion = 3;
		glesContextVersion = EGL_OPENGL_ES3_BIT_KHR;
		break;
	default:
		break;
	}

	m_eglNativeWindow = static_cast<EGLNativeWindowType>(parameters.WinId);
	
#ifndef SHADOW_APPLE
	EGLConfig config;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, glesApiVersion, EGL_NONE };

#ifdef SHADOW_ANDROID
	// For Android, get the width/height from the window rather than what the
	// application requested.
	//	esContext->width = ANativeWindow_getWidth(m_eglNativeWindow);
	//	esContext->height = ANativeWindow_getHeight(m_eglNativeWindow);
#else
	m_width = parameters.width;
	m_height = parameters.height;
#endif

	m_eglDisplay = eglGetDisplay(m_eglNativeDisplay);

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

	if (glesContextVersion == EGL_OPENGL_ES3_BIT_KHR && supportedContext != glesContextVersion)
	{
		// Error: this context (OpenGL ES 3.0) is not supported
		exit(0);
	}

	{
		EGLint numConfigs = 0;
		EGLint attribList[] =
		{
			EGL_RED_SIZE,       8,
			EGL_GREEN_SIZE,     8,
			EGL_BLUE_SIZE,      8,
			EGL_ALPHA_SIZE,     8,
			EGL_DEPTH_SIZE,		24,
			EGL_STENCIL_SIZE,   8,
			EGL_SAMPLE_BUFFERS, 0,
			// if EGL_KHR_create_context extension is supported, then we will use
			// EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
			EGL_RENDERABLE_TYPE, glesContextVersion,
			EGL_NONE
		};

		// Choose config
		if (!eglChooseConfig(m_eglDisplay, attribList, &config, 1, &numConfigs))
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
		//		EGLint format = 0;
		//		eglGetConfigAttrib(m_eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
		//		ANativeWindow_setBuffersGeometry(m_eglNativeWindow, 0, 0, format);
	}
#endif // ANDROID

	// Create a surface
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, m_eglNativeWindow, NULL);

	if (m_eglSurface == EGL_NO_SURFACE)
	{
		return GL_FALSE;
	}

	// Create a GL context
	m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, contextAttribs);

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
	return true;
}


bool EGLContextManager::SwapBuffers()
{
	return (eglSwapBuffers(m_eglDisplay, m_eglSurface) == EGL_TRUE);
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