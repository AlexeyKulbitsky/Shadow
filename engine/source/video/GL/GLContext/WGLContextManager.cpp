#include "WGLContextManager.h"

#include <gl/glew.h>
#include <gl/wglew.h>


namespace sh
{

namespace video
{
	LRESULT CALLBACK MsgHandlerSimpleOpenGLClass(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		switch (uiMsg)
		{
		case WM_PAINT:
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam); // Default window procedure
		}
		return 0;
	}

	WGLContextManager::WGLContextManager()
	{
		HINSTANCE hInstance = GetModuleHandle(0);

		// Registering fake class
		WNDCLASSEX wc;

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
		wc.lpfnWndProc = (WNDPROC)MsgHandlerSimpleOpenGLClass;
		wc.cbClsExtra = 0; wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_MENUBAR + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = "Fake_class_name";

		RegisterClassEx(&wc);

		HWND hWndFake = CreateWindow("Fake_class_name", "FAKE", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
			0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
			NULL, hInstance, NULL);

		HDC hDC = GetDC(hWndFake);

		// First, choose false pixel format

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
		if (iPixelFormat == 0)
			return;

		if (!SetPixelFormat(hDC, iPixelFormat, &pfd))
			return;

		// Create the false, old style context (OpenGL 2.1 and before)

		HGLRC hRCFake = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRCFake);

		auto res = glewInit();
		SH_ASSERT(res == GLEW_OK, "Can not initialize GLEW!");

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRCFake);
		DestroyWindow(hWndFake);
	}

	bool WGLContextManager::InitContext(const CreationParameters &parameters)
	{
		

		return false;
	}

	bool WGLContextManager::AttachWindow(void* window)
	{
		return false;
	}

	bool WGLContextManager::CreateDisplay()
	{
		return false;
	}

	bool WGLContextManager::DestroyDisplay()
	{
		return false;
	}

	bool WGLContextManager::CreateContext(bool createDisplay)
	{
		return false;
	}

	bool WGLContextManager::CreateContext(void* winId)
	{
		if (m_hwnd)
			DestroyContext(false);

		m_hwnd = reinterpret_cast<HWND>(winId);
		m_hdc = GetDC(m_hwnd);

		// Create OpenGL context
		const int majorVersion = 3;
		const int minorVersion = 3;

		if (WGLEW_ARB_create_context && WGLEW_ARB_pixel_format)
		{
			const int iPixelFormatAttribList[] =
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB, 32,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_STENCIL_BITS_ARB, 8,
				0 // End of attributes list
			};
			int iContextAttribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
				WGL_CONTEXT_MINOR_VERSION_ARB, minorVersion,
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				0 // End of attributes list
			};

			PIXELFORMATDESCRIPTOR pfd;

			int iPixelFormat, iNumFormats;
			wglChoosePixelFormatARB(m_hdc, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);

			// PFD seems to be only redundant parameter now
			if (!SetPixelFormat(m_hdc, iPixelFormat, &pfd))
				return false;

			m_hrc = wglCreateContextAttribsARB(m_hdc, 0, iContextAttribs);
			// If everything went OK
			if (m_hrc)
				wglMakeCurrent(m_hdc, m_hrc);
		}
	}

	bool WGLContextManager::DestroyContext(bool destroyDisplay)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hrc);
		ReleaseDC(m_hwnd, m_hdc);
		m_hwnd = NULL;
		m_hrc = NULL;
		m_hdc = NULL;
		return false;
	}

	bool WGLContextManager::SwapBuffers()
	{
		::SwapBuffers(m_hdc);
		return false;
	}

	bool WGLContextManager::IsContextCreated()
	{
		return false;
	}


} // video

} // sh