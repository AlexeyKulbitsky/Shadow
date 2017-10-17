#ifndef SHADOW_WGLCONTEXT_MANAGER_INCLUDE
#define SHADOW_WGLCONTEXT_MANAGER_INCLUDE

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

		virtual bool InitContext(const CreationParameters &parameters) override;
		virtual bool AttachWindow(void* window) override;
		virtual bool CreateDisplay() override;
		virtual bool DestroyDisplay() override;
		virtual bool CreateContext(bool createDisplay) override;
		virtual bool CreateContext(void* winId) override;
		virtual bool DestroyContext(bool destroyDisplay) override;
		virtual bool SwapBuffers() override;
		virtual bool IsContextCreated() override;

	private:
		HWND m_hwnd;
		HDC m_hdc;
		HGLRC m_hrc;
	};

} // video

} // sh

#endif
