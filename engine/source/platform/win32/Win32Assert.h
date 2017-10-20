#ifndef SHADOW_WIN32_ASSERT_INCLUDE
#define SHADOW_WIN32_ASSERT_INCLUDE

#include "../../Globals.h"
#include <windows.h>

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
	sh::String levelstr;
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

	char caption[1024];
	sprintf_s(caption, 1024, "%s!!!", levelstr.c_str());


	int count = _scprintf("message: %s\nline: %d\nfile: %s\nexpression: %s\nfunction: %s", message, line, file, expression, function);
	std::string newMsg;
	newMsg.resize(count);//null terminated string
	if (message)
	{
		sprintf_s(&newMsg[0], count, "message: %s\nline: %d\nfile: %s\nexpression: %s\nfunction: %s", message, line, file, expression, function);
	}
	else
	{
		sprintf_s(&newMsg[0], count, "line: %d\nfile: %s\nexpression: %s\nfunction: %s", line, file, expression, function);
	}

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

#endif