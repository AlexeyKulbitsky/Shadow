#include "ShAssert.h"

#include <cstdio>
#include <cstdarg>
#include <string>
#include <set>
#include "CompileConfig.h"
#include <windows.h>

///////////////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4996)

std::set<std::string> s_assertOnceMessages;

///////////////////////////////////////////////////////////////////////////////

assertLib::AssertType DefaultHandler(const char* condition, const char* msg,
	const char* file, const int line)
{
	std::printf("%s(%d): Assert Failure: ", file, line);

	if (condition != NULL)
		std::printf("'%s' ", condition);

	if (msg != NULL)
		std::printf("%s", msg);

	std::printf("\n");

	return assertLib::AssertType::CONTINUE;
}

///////////////////////////////////////////////////////////////////////////////

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

bool ShowMessageBox(const char* expression, const char* file, int line, const char* msg)
{
#if defined(SHADOW_WINDOWS)
#include <mmsystem.h>
#include <winsock.h>

	char caption[256];
	sprintf(caption, "Error!!!: %d %s", line, file);

	//sprintf(newMsg, "%s\n\nexp: %s", msg, expression);
	int count = _scprintf("%s\n\nexp: %s", msg, expression);
	std::string newMsg;
	newMsg.resize(count);//null terminated string
	if (msg)
	{
		sprintf(&newMsg[0], "%s\n\nexp: %s", msg, expression);
	}
	else
	{
		sprintf(&newMsg[0], "exp: %s", expression);
	}


// 	StringStackWalker sw;
// 	sw.ShowCallstack();
// 	std::string stackText;
// 	stackText.append(sw.GetString());

	size_t newSize = newMsg.size();
	newMsg.resize(newSize + 1);
	newMsg[newSize] = 0;

	//int value = ::MessageBox (0, newMsg.c_str(), caption, MB_ICONEXCLAMATION |  MB_ABORTRETRYIGNORE | MB_TASKMODAL | MB_SETFOREGROUND);
	int value = ::CustomMessageBox(0, newMsg.c_str(), caption, "empty",MB_ICONEXCLAMATION | MB_ABORTRETRYIGNORE | MB_TASKMODAL | MB_SETFOREGROUND);

	if (value == IDABORT)
		;
	else if (value == IDRETRY)
		return true;
	else if (value == IDIGNORE)
		return false;

	return true;

#elif defined(SHADOW_ANDROID)	
	char caption[256];
	sprintf(caption, "Error!!!: %d %s", line, core::ExtractFilename(file));

	char newMsg[2048];
	if (msg)
	{
		sprintf(newMsg, "%s\n\nexp: %s", msg, expression);
	}
	else
	{
		sprintf(newMsg, "exp: %s", expression);
	}

	return Android_ShowAssertAlert(caption, newMsg);

#elif defined(SHADOW_APPLE)

	char caption[256];
	sprintf(caption, "Error!!!: %d %s", line, core::ExtractFilename(file));

	//sprintf(newMsg, "%s\n\nexp: %s", msg, expression);
	int count = scprintf("%s\n\nexp: %s", msg, expression);
	std::string newMsg;
	newMsg.resize(count);//null terminated string
	if (msg)
	{
		sprintf(&newMsg[0], "%s\n\nexp: %s", msg, expression);
	}
	else
	{
		sprintf(&newMsg[0], "exp: %s", expression);
	}
	printf("%s", newMsg.c_str());
	fflush(stdout);
	System_ShowMessageBox(caption, newMsg);

	return false;

#elif defined(SHADOW_LINUX)

#else
#error define a print for your platform
#endif

	return false;
}

///////////////////////////////////////////////////////////////////////////////

assertLib::AssertType ShadowDefaultHandler(const char* condition, const char* msg,
	const char* file, const int line)
{
	bool b = ShowMessageBox(condition, file, line, msg);

#if defined(SHADOW_ANDROID)
	// force a crash to get stack trace info
	// (disabled, now printing native dialog)
	//void (*fn)() = 0;
	//(*fn)();
#endif

	return b ? assertLib::AssertType::BREAK : assertLib::AssertType::CONTINUE;
}

///////////////////////////////////////////////////////////////////////////////

static assertLib::AssertHandler s_handler = &DefaultHandler;
//static assertLib::AssertHandler s_handler = &ShadowDefaultHandler;

///////////////////////////////////////////////////////////////////////////////

void assertLib::SetAssertHandler(assertLib::AssertHandler h)
{
	s_handler = h;
}

///////////////////////////////////////////////////////////////////////////////

assertLib::AssertType assertLib::TestCondition(const char* condition, const char* file,
	int line, const char* msg, ...)
{
	char messageBuffer[8192];
	const char* message = NULL;
	if (msg != NULL)
	{
		{
			va_list args;
			va_start(args, msg);
			vsnprintf(messageBuffer, 8192, msg, args);
			va_end(args);
		}

		message = messageBuffer;
	}

	return s_handler(condition, message, file, line);
}

///////////////////////////////////////////////////////////////////////////////

assertLib::AssertType assertLib::TestConditionOnce(const char* condition, const char* file,
	int line, const char* msg, ...)
{
	char messageBuffer[8192];
	const char* message = NULL;
	if (msg != NULL)
	{
		{
			va_list args;
			va_start(args, msg);
			vsnprintf(messageBuffer, 8192, msg, args);
			va_end(args);
		}

		message = messageBuffer;
	}

	if (message == NULL)
	{
		sprintf(messageBuffer, "%s:%d", file, line);
		std::string tmp = messageBuffer;
		if (s_assertOnceMessages.find(tmp) != s_assertOnceMessages.end())
		{
			return assertLib::AssertType::CONTINUE;
		}
		s_assertOnceMessages.insert(tmp);
	}
	else
	{
		std::string tmp = message;
		if (s_assertOnceMessages.find(tmp) != s_assertOnceMessages.end())
		{
			return assertLib::AssertType::CONTINUE;
		}
		s_assertOnceMessages.insert(tmp);
	}

	return s_handler(condition, message, file, line);
}

