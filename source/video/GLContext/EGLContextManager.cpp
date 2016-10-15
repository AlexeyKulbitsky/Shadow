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
	m_eglNativeWindow = static_cast<EGLNativeWindowType>(parameters.WinId);

#ifndef SHADOW_APPLE
	EGLConfig config;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

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

	{
		EGLint numConfigs = 0;
		EGLint attribList[] =
		{
			EGL_RED_SIZE,       5,
			EGL_GREEN_SIZE,     6,
			EGL_BLUE_SIZE,      5,
			EGL_ALPHA_SIZE,     EGL_DONT_CARE,
			EGL_DEPTH_SIZE,     EGL_DONT_CARE,
			EGL_STENCIL_SIZE,   EGL_DONT_CARE,
			EGL_SAMPLE_BUFFERS, 0,
			// if EGL_KHR_create_context extension is supported, then we will use
			// EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
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