#ifndef SHADOW_WGLCONTEXT_MANAGER_INCLUDE
#define SHADOW_WGLCONTEXT_MANAGER_INCLUDE

#if defined (SHADOW_WINDOWS)


#include "GLContextManager.h"

#include <Windows.h>

namespace sh
{

namespace video
{

	class WGLContextManager : public GLContextManager
	{
	public:
		WGLContextManager();
		WGLContextManager(int majorVersion, int minorVersion);

		virtual bool CreateContext(void* winId) override;
		virtual bool DestroyContext() override;
		virtual void SwapBuffers() override;
		virtual bool IsContextCreated() override;

	private:
		void CreateOldWayContext();
		void CreateFakeWindow();
		void CreateNewWayContext();

	private:
		HWND m_hwnd;
		HDC m_hdc;
		HGLRC m_hrc;

		int m_minorVersion;
		int m_majorVersion;
	};

} // video

} // sh

#endif

#endif
