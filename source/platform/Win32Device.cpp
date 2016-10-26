#include "Win32Device.h"
#include "../video/Driver.h"
#include "../video/GLContext/EGLContextManager.h"
#include "../video/GLES20/GLES20Driver.h"

#include "../video/Vulkan/VulkanDriver.h"

using namespace sh;
using namespace video;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{

	}
	return 0;

	case WM_ERASEBKGND:
		return 0;

	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
	}

	case WM_SIZE:
	{

	}
	return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SYSCOMMAND:
	case WM_ACTIVATE:
	case WM_USER:
	case WM_SETCURSOR:
	case WM_INPUTLANGCHANGE:
	{

	}

	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

Win32Device::Win32Device()
{

}

////////////////////////////////////////////////////////////////////////

Win32Device::Win32Device(const CreationParameters &parameters)
	:Device(parameters)
{

	// get handle to exe file
	HINSTANCE hInstance = GetModuleHandle(0);

	// Store original desktop mode.

	//memset(&DesktopMode, 0, sizeof(DesktopMode));
	//DesktopMode.dmSize = sizeof(DesktopMode);

	//EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DesktopMode);

	// create the window if we need to and we do not use the null device
	if (!m_creationParameters.WinId)
	{
		const fschar_t* ClassName = __TEXT("Win32Device");

		// Register Class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = 0; // LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = 0;
		wcex.lpszClassName = ClassName;
		wcex.hIconSm = 0;

		// if there is an icon, load it
		wcex.hIcon = (HICON)LoadImage(hInstance, __TEXT("shadow.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

		RegisterClassEx(&wcex);

		// calculate client size

		RECT clientSize;
		clientSize.top = 0;
		clientSize.left = 0;
		clientSize.right = m_creationParameters.width;
		clientSize.bottom = m_creationParameters.height;

		DWORD style = WS_POPUP;
	
		style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		AdjustWindowRect(&clientSize, style, FALSE);

		const s32 realWidth = clientSize.right - clientSize.left;
		const s32 realHeight = clientSize.bottom - clientSize.top;

		s32 windowLeft = 100;
		s32 windowTop = 100;

		if (windowLeft < 0)
			windowLeft = 0;
		if (windowTop < 0)
			windowTop = 0;	// make sure window menus are in screen on creation


		// create window
		m_hwnd = CreateWindow(ClassName, __TEXT("Shadow engine"), style, windowLeft, windowTop,
			realWidth, realHeight, NULL, NULL, hInstance, NULL);
		

		m_creationParameters.WinId = m_hwnd;

		ShowWindow(m_hwnd, SW_SHOWNORMAL);
		UpdateWindow(m_hwnd);

		// fix ugly ATI driver bugs. Thanks to ariaci
		MoveWindow(m_hwnd, windowLeft, windowTop, realWidth, realHeight, TRUE);

		// make sure everything gets updated to the real sizes	
	}
	else if (m_creationParameters.WinId)
	{
		// attach external window
		m_hwnd = static_cast<HWND>(m_creationParameters.WinId);
		RECT r;
		GetWindowRect(m_hwnd, &r);
		m_creationParameters.width = r.right - r.left;
		m_creationParameters.height = r.bottom - r.top;
	}

	CreateDriver();
}

////////////////////////////////////////////////////////////////////////

Win32Device::~Win32Device()
{

}

////////////////////////////////////////////////////////////////////////

bool Win32Device::CreateDriver()
{
	switch (m_creationParameters.driverType)
	{
	case video::DriverType::OPENGL_ES_2_0:
	{
		video::EGLContextManager* contextManager = new video::EGLContextManager();
		if (contextManager)
		{
			contextManager->InitContext(m_creationParameters);
			m_driver = new video::GLES20Driver(contextManager);
			m_driver->Init();

			m_GLContextManager = contextManager;
		}
	}
	break;
	case video::DriverType::VULKAN:
	{
		m_driver = new video::VulkanDriver(m_creationParameters);
		m_driver->Init();
	}
	break;
	default:
		break;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////