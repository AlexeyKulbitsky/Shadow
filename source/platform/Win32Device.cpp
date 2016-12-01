#include "Win32Device.h"
#include "../video/Driver.h"
#include "../video/GLContext/EGLContextManager.h"
#include "../video/GLES20/GLES20Driver.h"
#include "../video/Vulkan/VulkanDriver.h"
#include "../scene/SceneManager.h"

using namespace sh;
using namespace video;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Device* device = Device::GetInstance();

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		Event e;
		e.type = EventType::MOUSE_INPUT_EVENT;
		e.mouseEvent.x = x;
		e.mouseEvent.y = y;
		e.mouseEvent.type = MouseEventType::LMB_PRESSED;
		if (device)
			device->OnEvent(e);
	}
		break;

	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		Event e;
		e.type = EventType::MOUSE_INPUT_EVENT;
		e.mouseEvent.x = x;
		e.mouseEvent.y = y;
		e.mouseEvent.type = MouseEventType::LMB_RELEASED;
		if (device)
			device->OnEvent(e);
	}
		break;
	case WM_MOUSEMOVE:
	{
		bool isShiftPressed = (LOWORD(wParam) & MK_SHIFT) != 0;
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		Event e;
		e.type = EventType::MOUSE_INPUT_EVENT;
		e.mouseEvent.x = x;
		e.mouseEvent.y = y;
		e.mouseEvent.type = MouseEventType::MOVED;
		if (device)
			device->OnEvent(e);
	}
		break;
	case WM_PAINT:
		break;

	case WM_ERASEBKGND:
		break;

	case WM_SYSKEYDOWN:
		break;
	case WM_SYSKEYUP:
		break;
	case WM_KEYDOWN:
	{
		Event e;
		e.type = EventType::KEYBOARD_INPUT_EVENT;
		e.keyboardEvent.type = KeyboardEventType::KEY_PRESEED;
		e.keyboardEvent.keyCode = (KeyCode)wParam;
		if (device)
			device->OnEvent(e);
	}
		break;
	case WM_KEYUP:
	{
		Event e;
		e.type = EventType::KEYBOARD_INPUT_EVENT;
		e.keyboardEvent.type = KeyboardEventType::KEY_RELEASED;
		e.keyboardEvent.keyCode = (KeyCode)wParam;
		if (device)
			device->OnEvent(e);
	}
		break;

	case WM_SIZE:
	{
		printf("Resize");
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

bool Win32Device::Run()
{
	/*
	static char capture[128];
	static u64 t = 0;
	sprintf(capture, "Shdow engine: FPS %d", GetTime() - t);
	t = GetTime();
	SetWindowText(m_hwnd, capture);
	*/

	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	//while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			m_needsToClose = true;
			//done = true;
		}
	}

	return !m_needsToClose;
}

////////////////////////////////////////////////////////////////////////

void Win32Device::OnEvent(const Event& e)
{
	switch (e.type)
	{
	case EventType::MOUSE_INPUT_EVENT:
	{
		switch (e.mouseEvent.type)
		{
		case MouseEventType::LMB_PRESSED:
		{
			//printf("LMB pressed, pos: x=%d y=%d\n", e.mouseEvent.x, e.mouseEvent.y);
		}
			break;
		case MouseEventType::LMB_RELEASED:
		{
			//printf("LMB release, pos: x=%d y=%d\n", e.mouseEvent.x, e.mouseEvent.y);
		}
			break;
		case MouseEventType::MOVED:
		{
			//printf("Mouse move, pos: x=%d y=%d\n", e.mouseEvent.x, e.mouseEvent.y);
		}
			break;
		default:
			break;
		}
	}
		break;
	case EventType::KEYBOARD_INPUT_EVENT:
	{
		m_sceneManager->OnEvent(e);
	}
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////

u64 Win32Device::GetTime()
{
	static LARGE_INTEGER freq;
	static LARGE_INTEGER crt;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&crt);
	return (crt.QuadPart * 1000000) / freq.QuadPart;
}

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