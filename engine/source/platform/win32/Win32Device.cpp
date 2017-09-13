#include "Win32Device.h"
#include "../../video/Driver.h"
#include "../../video/GLContext/EGLContextManager.h"
#include "../../video/GLES20/GLES20Driver.h"

#if defined (SHADOW_USE_VULKAN_API)
#include "../../video/Vulkan/VulkanDriver.h"
#endif

#include "../../scene/SceneManager.h"
#include "../../Application.h"

#include "../../gui/GuiManager.h"

#include "../../io/win32/WindowsFileSystem.h"

#include <chrono>

// Window assert implementation
#include "Win32Assert.h"

#include "../../Shadow.h"

using namespace sh;
using namespace video;


int g_xPoint, g_yPoint;
#include <shellapi.h>

void ProcessDropMessage(HDROP dropInfo);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Device* device = Device::GetInstance();

	static int old = 0;
	
	//SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &old, 0);
	//SystemParametersInfo(SPI_SETKEYBOARDSPEED,0, &old, 0);
	
	//SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &old, 0);
	//SystemParametersInfo(SPI_SETKEYBOARDDELAY,0, &old, 0);

	switch (message)
	{
		case WM_DROPFILES:
		{
			HDROP hDropInfo = (HDROP)wParam;
			ProcessDropMessage(hDropInfo);
		}
		return 0;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (device)
			device->mouseEvent(x, y, MouseEventType::ButtonPressed, MouseCode::ButtonLeft);
	}
		return 0;

	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (device)
			device->mouseEvent(x, y, MouseEventType::ButtonReleased, MouseCode::ButtonLeft);
	}
		return 0;
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (device)
			device->mouseEvent(x, y, MouseEventType::ButtonPressed, MouseCode::ButtonWheel);
	}
		return 0;
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (device)
			device->mouseEvent(x, y, MouseEventType::ButtonReleased, MouseCode::ButtonWheel);
	}
		return 0;
	case WM_MOUSEMOVE:
	{
		bool isShiftPressed = (LOWORD(wParam) & MK_SHIFT) != 0;
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		g_xPoint = x;
		g_yPoint = y;

		if (device)
			device->mouseEvent(x, y, MouseEventType::Moved, MouseCode::ButtonLeft);
	}
		return 0;
	case WM_MOUSEHWHEEL:
	case WM_MOUSEWHEEL:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		ScreenToClient(hWnd, &pt);

		g_xPoint = pt.x;
		g_yPoint = pt.y;

		short delta = (short)HIWORD(wParam);
		int d = delta < 0 ? -1 : 1;
		
		if (device)
			device->mouseWheelEvent(pt.x, pt.y, d);
	}
		return 0;
	case WM_PAINT:
		break;

	case WM_ERASEBKGND:
		break;

	case WM_SYSKEYDOWN:
		break;
	case WM_SYSKEYUP:
		return 0;
	case WM_KEYDOWN:
	{
		if (device)
			device->keyboardEvent(KeyboardEventType::KeyPressed, (KeyCode)wParam);
	}
		return 0;
	case WM_KEYUP:
	{
		if (device)
			device->keyboardEvent(KeyboardEventType::KeyReleased, (KeyCode)wParam);
	}
		return 0;

	case WM_SIZE:
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		if (device)
			device->windowResizeEvent(width, height);
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

	if (device)
	{
		KeyboardEventType ev = KeyboardEventType::KeyReleased;
		// Alt button
		GetKeyState(VK_MENU) & 0x8000 ? ev = KeyboardEventType::KeyPressed : KeyboardEventType::KeyReleased;
		device->keyboardEvent(ev, KeyCode::KEY_MENU);
		// Shift button
		GetKeyState(VK_SHIFT) & 0x8000 ? ev = KeyboardEventType::KeyPressed : KeyboardEventType::KeyReleased;
		device->keyboardEvent(ev, KeyCode::KEY_SHIFT);
		// Ctrl button
		GetKeyState(VK_CONTROL) & 0x8000 ? ev = KeyboardEventType::KeyPressed : KeyboardEventType::KeyReleased;
		device->keyboardEvent(ev, KeyCode::KEY_CONTROL);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////

Win32Device::Win32Device()
{
	io::FileSystem::CreateInstance<io::WindowsFileSystem>();
	m_fileSystem = io::FileSystem::GetInstance();
	m_fileSystem->AddFolder(sh::String("../../../data"));

	const auto& info = m_fileSystem->FindFile("config.xml");
	if (info.name != "")
	{
		auto file = m_fileSystem->LoadFile("config.xml");
		const char* dataPtr = file.GetData().data();

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_buffer(dataPtr, file.GetData().size());

		auto rootNode = doc.child("config");
		auto driverNode = rootNode.child("driver");
		String driverName = driverNode.attribute("type").as_string();
		if (driverName == "GLES20")
			m_creationParameters.driverType = video::DriverType::OPENGL_ES_2_0;
	}

	pempek::assert::implementation::setAssertHandler(_testHandler);

	// get handle to exe file
	HINSTANCE hInstance = GetModuleHandle(0);

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

	style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
		WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

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
	//m_hwnd = CreateWindow(ClassName, __TEXT("Shadow engine"), style, windowLeft, windowTop,
	//	realWidth, realHeight, NULL, NULL, hInstance, NULL);

	DWORD exStyle = WS_EX_ACCEPTFILES;
	m_hwnd = CreateWindowEx(exStyle, ClassName, __TEXT("Shadow engine"), style, windowLeft, windowTop,
		realWidth, realHeight, NULL, NULL, hInstance, NULL);


	m_creationParameters.WinId = m_hwnd;

	ShowWindow(m_hwnd, SW_SHOWNORMAL);
	UpdateWindow(m_hwnd);

	// fix ugly ATI driver bugs. Thanks to ariaci
	MoveWindow(m_hwnd, windowLeft, windowTop, realWidth, realHeight, TRUE);

	CreateDriver();

	//m_application = CreateApplication();

	//CallSomeExternFunction();
}

////////////////////////////////////////////////////////////////////////

Win32Device::Win32Device(const CreationParameters &parameters)
	:Device(parameters)
{
	pempek::assert::implementation::setAssertHandler(_testHandler);

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
	
		style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | 
			WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

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

	io::FileSystem::CreateInstance<io::WindowsFileSystem>();
	m_fileSystem = io::FileSystem::GetInstance();

	CreateDriver();
}

////////////////////////////////////////////////////////////////////////

Win32Device::~Win32Device()
{
	if (!!m_GLContextManager)
	{
		delete m_GLContextManager;
		m_GLContextManager = nullptr;
	}
	if (!!m_driver)
	{
		delete m_driver;
		m_driver = nullptr;
	}
	
	if (!!m_sceneManager)
	{
		delete m_sceneManager;
		m_sceneManager = nullptr;
	}
	
	io::FileSystem::DestroyInstance();
}

////////////////////////////////////////////////////////////////////////

void Win32Device::Init()
{
	m_driver->SetWindow(m_creationParameters.WinId, m_creationParameters.width, m_creationParameters.height);
	m_driver->Init();
	m_driver->SetViewport(0U, 0U, m_creationParameters.width, m_creationParameters.height);

	Device::Init();

	m_application->Init();
}

////////////////////////////////////////////////////////////////////////

void Win32Device::Update(f32 deltaTime)
{
	
}

////////////////////////////////////////////////////////////////////////

void Win32Device::Run()
{
	m_startTimePoint = GetTime();
	m_lastFrameTimePoint = m_startTimePoint;

	MSG msg;
	bool done;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		const u64 currentTimePoint = GetTime();
		const u64 delta = currentTimePoint - m_lastFrameTimePoint;

		m_application->Update(delta);

		m_lastFrameTimePoint = currentTimePoint;
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
			m_driver = new video::GLES20Driver(contextManager);
			m_GLContextManager = contextManager;
		}
	}
	break;
#if defined (SHADOW_USE_VULKAN_API)
	case video::DriverType::VULKAN:
	{
		m_driver = new video::VulkanDriver(m_creationParameters);
	}
	break;
#endif
	default:
		break;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////

void ProcessDropMessage(HDROP dropInfo)
{
	char sItem[MAX_PATH];
	for (int i = 0; DragQueryFile(dropInfo, i, (LPSTR)sItem, sizeof(sItem)); i++)
	{
		//Is the item a file or a directory?
		if (GetFileAttributes(sItem) &FILE_ATTRIBUTE_DIRECTORY)
		{
			printf("Folder %s\n", sItem);
		}
		else 
		{
			printf("File %s\n", sItem);
		}

	}
	DragFinish(dropInfo);
}
