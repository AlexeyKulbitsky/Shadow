#include "WGLContextManager.h"

#if defined (SHADOW_WINDOWS)

#pragma comment(lib, "opengl32.lib")

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
		
	}

	WGLContextManager::WGLContextManager(int majorVersion, int minorVersion)
		: m_majorVersion(majorVersion)
		, m_minorVersion(minorVersion)
	{
	}

	bool WGLContextManager::CreateContext(void* winId)
	{
		if (m_hwnd)
			DestroyContext();

		m_hwnd = reinterpret_cast<HWND>(winId);
		m_hdc = GetDC(m_hwnd);

		if (m_majorVersion < 3)
			CreateOldWayContext();
		else
			CreateNewWayContext();

		return true;
	}

	bool WGLContextManager::DestroyContext()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hrc);
		ReleaseDC(m_hwnd, m_hdc);
		m_hwnd = NULL;
		m_hrc = NULL;
		m_hdc = NULL;
		return false;
	}

	void WGLContextManager::SwapBuffers()
	{
		::SwapBuffers(m_hdc);
	}

	bool WGLContextManager::IsContextCreated()
	{
		return m_hrc != NULL;
	}

	void WGLContextManager::CreateOldWayContext()
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;

		int iPixelFormat = ChoosePixelFormat(m_hdc, &pfd);

		if (!SetPixelFormat(m_hdc, iPixelFormat, &pfd))
			return;

		m_hrc = wglCreateContext(m_hdc);

		if (m_hrc)
			wglMakeCurrent(m_hdc, m_hrc);

		auto res = glewInit();
		SH_ASSERT(res == GLEW_OK, "Can not initialize GLEW!");
	}

	void WGLContextManager::CreateFakeWindow()
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

	void WGLContextManager::CreateNewWayContext()
	{
		CreateFakeWindow();


		// Create OpenGL context
		if (WGLEW_ARB_create_context && WGLEW_ARB_pixel_format)
		{
			const int iPixelFormatAttribList[] =
			{
				WGL_SUPPORT_OPENGL_ARB, TRUE,
				WGL_DRAW_TO_WINDOW_ARB, TRUE,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_COLOR_BITS_ARB, 32,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_DOUBLE_BUFFER_ARB, TRUE,
				WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_STENCIL_BITS_ARB, 8,
				0 // End of attributes list
			};

			int iContextAttribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, m_majorVersion,
				WGL_CONTEXT_MINOR_VERSION_ARB, m_minorVersion,
				//WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				//WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0 // End of attributes list
			};

			PIXELFORMATDESCRIPTOR pfd;

			int iPixelFormat, iNumFormats;
			wglChoosePixelFormatARB(m_hdc, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);

			// PFD seems to be only redundant parameter now
			if (!SetPixelFormat(m_hdc, iPixelFormat, &pfd))
			{
				SH_ASSERT(0, "Can not set pixel formal while creating OpenGL context");
				return;
			}

			m_hrc = wglCreateContextAttribsARB(m_hdc, 0, iContextAttribs);
			// If everything went OK
			if (m_hrc)
			{
				wglMakeCurrent(m_hdc, m_hrc);
			}
			else
			{
				SH_ASSERT(0, "Can not create OpenGL context");
			}
		}
	}


} // video

} // sh

#endif
