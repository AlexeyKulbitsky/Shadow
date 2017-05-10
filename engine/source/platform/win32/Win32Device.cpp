#include "Win32Device.h"
#include "../video/Driver.h"
#include "../video/GLContext/EGLContextManager.h"
#include "../video/GLES20/GLES20Driver.h"
#include "../video/Vulkan/VulkanDriver.h"
#include "../scene/SceneManager.h"

#include "../../gui/GuiManager.h"

#include "../../io/win32/WindowsFileSystem.h"

#include <chrono>

using namespace sh;
using namespace video;






// Declare CBT hook procedure
static LRESULT CALLBACK CbtHookProc(int, WPARAM, LPARAM);
// Declare custom message box procedure
LRESULT CALLBACK CustomMessageBoxProc(HWND, UINT, WPARAM, LPARAM);



////////////////////////////////////////////////////////////////////////////////
// CustomMessageBoxValues structure                                           //
////////////////////////////////////////////////////////////////////////////////
// Keeps data along custom message box functions and the hook procedure       //
////////////////////////////////////////////////////////////////////////////////
struct CustomMessageBoxValues
{
	HHOOK hHook;          // hook handle
	HWND  hWnd;           // message box handle
	WNDPROC lpMsgBoxProc; // window procedure
	LPCTSTR box;
	HWND hButton;
	int heightShow, heightHide;
} static __declspec(thread) cmbv;

BOOL CenterWindow(HWND hwndChild)
{
	RECT    rChild, rWorkArea;
	int     wChild, hChild;
	int     xNew, yNew;
	BOOL    bResult;

	// Get the Height and Width of the child window
	GetWindowRect(hwndChild, &rChild);
	wChild = rChild.right - rChild.left;
	hChild = rChild.bottom - rChild.top;

	// Get the limits of the 'workarea'
	bResult = SystemParametersInfo(
		SPI_GETWORKAREA,    // system parameter to query or set
		sizeof(RECT),
		&rWorkArea,
		0);
	if (!bResult) {
		rWorkArea.left = rWorkArea.top = 0;
		rWorkArea.right = GetSystemMetrics(SM_CXSCREEN);
		rWorkArea.bottom = GetSystemMetrics(SM_CYSCREEN);
	}

	// Calculate new X position, then adjust for workarea
	xNew = (rWorkArea.right / 2) - wChild / 2;

	// Calculate new Y position, then adjust for workarea
	yNew = (rWorkArea.bottom / 2) - hChild / 2;

	// Set it, and return
	return SetWindowPos(hwndChild, NULL, xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

////////////////////////////////////////////////////////////////////////////////
// Custom message box function                                                //
////////////////////////////////////////////////////////////////////////////////
// Call this instead of MessageBox API function in order to display           //
// a customized message box.                                                  //
////////////////////////////////////////////////////////////////////////////////
int CustomMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, LPCTSTR textbox, UINT uType)
{
	// init CustomMessageBoxValues structure values
	cmbv.hHook = NULL;
	cmbv.hWnd = NULL;
	cmbv.lpMsgBoxProc = NULL;
	cmbv.box = textbox;

	HINSTANCE hInstance = ::GetModuleHandle(NULL);
	//  installs the hook procedure into a hook chain
	cmbv.hHook = SetWindowsHookEx(WH_CBT, CbtHookProc, hInstance, GetCurrentThreadId());

	// call "standard" MessageBox Windows API function 
	int nRet = MessageBox(hWnd, lpText, lpCaption, uType);

	// removes the hook procedure from the hook chain
	::UnhookWindowsHookEx(cmbv.hHook);

	return nRet;
}



LRESULT CALLBACK CustomMessageBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		LRESULT ret = CallWindowProc(cmbv.lpMsgBoxProc, hWnd, uMsg, wParam, lParam);

		HINSTANCE hInstance = ::GetModuleHandle(NULL);
		RECT r;
		GetClientRect(hWnd, &r);
		HGDIOBJ obj = ::GetStockObject(DEFAULT_GUI_FONT);

		// create show details button
		int buttonHeight = 30;
		int margin = 10;
		HWND hButton = ::CreateWindowExW(WS_EX_CLIENTEDGE
			, L"BUTTON", L"Show details"
			, WS_CHILD | WS_VISIBLE | WS_TABSTOP
			, r.right - 100 - margin, r.bottom, 100, buttonHeight,
			hWnd, NULL, hInstance, NULL);

		cmbv.hButton = hButton;

		::SendMessage(hButton, WM_SETFONT, (WPARAM)obj, MAKELPARAM(FALSE, 0));

		// create an edit box
		int boxHeight = 300;
		HWND hBox = ::CreateWindowExW(WS_EX_CLIENTEDGE
			, L"EDIT", NULL
			, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | WS_TABSTOP// | ES_AUTOHSCROLL 
			, 10, r.bottom + buttonHeight + margin, (r.right - r.left) - 20, boxHeight,
			hWnd, NULL, hInstance, NULL);

		::SetWindowText(hBox, cmbv.box);

		// change edit box font
		::SendMessage(hBox, WM_SETFONT, (WPARAM)obj, MAKELPARAM(FALSE, 0));

		// set read only property
		//::SendMessage(hBox, EM_SETREADONLY, (WPARAM) TRUE, 0L);//redraw problems

		GetWindowRect(hWnd, &r);
		cmbv.heightShow = (r.bottom - r.top) + buttonHeight + margin;
		cmbv.heightHide = (r.bottom - r.top) + buttonHeight + margin + boxHeight + margin;

		// resize the dialog
		SetWindowPos(hWnd, 0, 0, 0, (r.right - r.left), cmbv.heightShow, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		CenterWindow(hWnd);

		// disable default push button
		::SendMessage(hWnd, DM_SETDEFID, 0, 0);
		return ret;

	}
	break;

	case WM_COMMAND:
	{
		int code = HIWORD(wParam);
		switch (code)
		{
		case BN_CLICKED:
		{
			int buttonid = LOWORD(wParam);
			HWND hButton = GetDlgItem(hWnd, buttonid);
			if (hButton == cmbv.hButton)
			{
				RECT r;
				GetWindowRect(hWnd, &r);
				int h = ((r.bottom - r.top) == cmbv.heightHide) ? cmbv.heightShow : cmbv.heightHide;
				SetWindowText(GetDlgItem(hWnd, buttonid),
					(h == cmbv.heightShow) ? "Show details" : "Hide details");
				SetWindowPos(hWnd, 0, r.left, r.top, (r.right - r.left), h, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				break;
			}
		}

		break;
		default:
			break;
		}
	}
	break;
	}
	return CallWindowProc(cmbv.lpMsgBoxProc, hWnd, uMsg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////
// CBT (computer-based training) hook procedure                               //
////////////////////////////////////////////////////////////////////////////////
// Catch the dialog box creation in order to subclass it                      //
//////////////////////////////////////////////////////////////////////////////// 
LRESULT CALLBACK CbtHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
	{
		return ::CallNextHookEx(cmbv.hHook, nCode, wParam, lParam);
	}
	switch (nCode)
	{
	case HCBT_CREATEWND: // a window is about to be created
	{
		LPCBT_CREATEWND lpCbtCreate = (LPCBT_CREATEWND)lParam;
		if (WC_DIALOG == lpCbtCreate->lpcs->lpszClass)
		{
			// WC_DIALOG is the class name of message box
			// but it has not yet a window procedure set.
			// So keep in mind the handle to sublass it later
			// when its first child is about to be created
			cmbv.hWnd = (HWND)wParam;
		}
		else
		{
			if ((NULL == cmbv.lpMsgBoxProc) && (NULL != cmbv.hWnd))
			{
				// subclass the dialog 
				cmbv.lpMsgBoxProc =
					(WNDPROC)::SetWindowLongPtr(cmbv.hWnd, GWLP_WNDPROC, (LONG_PTR)CustomMessageBoxProc);
			}
		}
	}
	break;
	case HCBT_DESTROYWND: // a window is about to be destroyed
	{
		if (cmbv.hWnd == (HWND)wParam) // it's our messge box
		{
			// so set back its default procedure
			::SetWindowLongPtr(cmbv.hWnd, GWLP_WNDPROC, (LONG_PTR)cmbv.lpMsgBoxProc);
		}
	}
	}
	return 0;
}

int ShowMessageBox(const char* file, int line, const char* function, const char* expression, int level, const char* message)
//int ShowMessageBox(const char* expression, const char* file, int line, const char* msg)
{
	String levelstr;
	switch (level)
	{
	case pempek::assert::implementation::AssertLevel::Debug:
		levelstr = "DEBUG";
		break;
	case pempek::assert::implementation::AssertLevel::Warning:
		levelstr = "WARNING";
		break;
	case pempek::assert::implementation::AssertLevel::Error:
		levelstr = "ERROR";
		break;
	case pempek::assert::implementation::AssertLevel::Fatal:
		levelstr = "FATAL";
		break;

	default:
		break;
	}

	char caption[256];
	sprintf(caption, "%s!!!", levelstr.c_str());

	
	int count = _scprintf("message: %s\nline: %d\nfile: %s\nexpression: %s\nfunction: %s", message, line, file, expression, function);
	std::string newMsg;
	newMsg.resize(count);//null terminated string
	if (message)
	{
		sprintf(&newMsg[0], "message: %s\nline: %d\nfile: %s\nexpression: %s\nfunction: %s", message, line, file, expression, function);
	}
	else
	{
		sprintf(&newMsg[0], "line: %d\nfile: %s\nexpression: %s\nfunction: %s", line, file, expression, function);
	}
	


	//FileStream fs;

	//if (fs.Open(k_assertsFilename, FileStream::k_app))
	//ms can show call stack in window
	//StringStackWalker sw;
	//sw.ShowCallstack();
	//std::string stackText;
	//stackText.append(sw.GetString());

	size_t newSize = newMsg.size();
	newMsg.resize(newSize + 1);
	newMsg[newSize] = 0;

	int value = ::CustomMessageBox(0, newMsg.c_str(), caption, "Place for call stack"/*stackText.c_str()*/, MB_ICONEXCLAMATION | MB_ABORTRETRYIGNORE | MB_TASKMODAL | MB_SETFOREGROUND);

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pempek::assert::implementation::AssertAction::AssertAction _testHandler(const char* file,
                                          int line,
                                          const char* function,
                                          const char* expression,
                                          int level,
                                          const char* message)
  {
    

	  int msgboxID = ShowMessageBox(file, line, function, expression, level, message);

	  pempek::assert::implementation::AssertAction::AssertAction res = pempek::assert::implementation::AssertAction::AssertAction::None;

	  switch (msgboxID)
	  {
	  case IDABORT:
		  // TODO: add code
		  res = pempek::assert::implementation::AssertAction::AssertAction::Abort;
		  break;
	  case IDRETRY:
		  // TODO: add code
		  res = pempek::assert::implementation::AssertAction::AssertAction::Ignore;
		  break;
	  case IDIGNORE:
		  // TODO: add code
		  res = pempek::assert::implementation::AssertAction::AssertAction::IgnoreAll;
		  break;
	  }

	  return res;

  }

int g_xPoint, g_yPoint;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Device* device = Device::GetInstance();

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (device)
			device->mouseEvent(x, y, MouseEventType::ButtonPressed, MouseCode::ButtonLeft);
	}
		break;

	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (device)
			device->mouseEvent(x, y, MouseEventType::ButtonReleased, MouseCode::ButtonLeft);
	}
		break;
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (device)
			device->mouseEvent(x, y, MouseEventType::ButtonPressed, MouseCode::ButtonWheel);
	}
		break;
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (device)
			device->mouseEvent(x, y, MouseEventType::ButtonReleased, MouseCode::ButtonWheel);
	}
		break;
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
		break;
	case WM_MOUSEHWHEEL:
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		g_xPoint = x;
		g_yPoint = y;

		short delta = (short)HIWORD(wParam);
		int d = delta < 0 ? -1 : 1;
		
		if (device)
			device->mouseWheelEvent(d);
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
		if (device)
			device->keyboardEvent(KeyboardEventType::KeyPressed, (KeyCode)wParam);
	}
		break;
	case WM_KEYUP:
	{
		if (device)
			device->keyboardEvent(KeyboardEventType::KeyReleased, (KeyCode)wParam);
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

	io::FileSystem::CreateInstance<io::WindowsFileSystem>();
	m_fileSystem = io::FileSystem::GetInstance();

	CreateDriver();
	//m_driver->SetViewport(0, 0, m_creationParameters.width, m_creationParameters.height);
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
	CreateWindowContext();
	m_driver->Init();
}

////////////////////////////////////////////////////////////////////////

void Win32Device::Update(f32 deltaTime)
{
	
}

////////////////////////////////////////////////////////////////////////

bool Win32Device::Run()
{
	///*
	static char capture[128];
	static auto t = std::chrono::high_resolution_clock::now();


	u32 delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t).count();
	float fps = 1000.0f / static_cast<float>(delta);
	sprintf(capture, "Shadow engine: %s API | FPS %.2f | x: %d y: %d", m_driver->GetApiName().c_str(), fps, g_xPoint, g_yPoint);
	t = std::chrono::system_clock::now();
	SetWindowText(m_hwnd, capture);
	//*/

	MSG msg;
	bool done;

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

u64 Win32Device::GetTime()
{
	static LARGE_INTEGER freq;
	static LARGE_INTEGER crt;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&crt);
	return (crt.QuadPart * 1000000) / freq.QuadPart;
}

void Win32Device::CreateWindowContext()
{
	if (!m_GLContextManager->IsContextCreated())
	{
		bool success = m_GLContextManager->CreateContext(true);
	}
	else
	{
		m_GLContextManager->DestroyContext(false);
		bool success = m_GLContextManager->CreateContext(false);
	}
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
			//contextManager->InitContext(m_creationParameters);
			m_driver = new video::GLES20Driver(contextManager);

			contextManager->AttachWindow(m_creationParameters.WinId);
			//m_driver->Init();

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