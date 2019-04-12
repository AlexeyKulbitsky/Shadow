#include "platform/win32/Win32Device.h"
#include "device/Application.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

////////////////////////////////////////////////////////////////////////

namespace sh
{
    Win32Device::Win32Device()
    {
        HINSTANCE hInstance = GetModuleHandle(0);
        m_hwnd = _CreateWindow(hInstance);
        m_creationParameters.WinId = m_hwnd;
    }

    ////////////////////////////////////////////////////////////////////////

    Win32Device::Win32Device(const CreationParameters &parameters)
        :Device(parameters)
    {
        // get handle to exe file
        HINSTANCE hInstance = GetModuleHandle(0);

        // create the window if we need to and we do not use the null device
        if (!m_creationParameters.WinId)
        {
            m_hwnd = _CreateWindow(hInstance);
            m_creationParameters.WinId = m_hwnd;
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
    }

    ////////////////////////////////////////////////////////////////////////

    Win32Device::~Win32Device()
    {
    }

    ////////////////////////////////////////////////////////////////////////

    void Win32Device::Init()
    {
        Device::Init();
        m_application->Init();
    }

    ////////////////////////////////////////////////////////////////////////

    void Win32Device::Update(float deltaTime)
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
            if (msg.message == WM_QUIT || m_application->NeedsToBeClosed())
            {
                done = true;
                m_application->Destroy();
                return;
            }
            const uint64_t currentTimePoint = GetTime();
            const uint64_t delta = currentTimePoint - m_lastFrameTimePoint;

            m_application->Update(delta);

            m_lastFrameTimePoint = currentTimePoint;
        }
    }

    ////////////////////////////////////////////////////////////////////////

    uint64_t Win32Device::GetTime()
    {
        static LARGE_INTEGER freq;
        static LARGE_INTEGER crt;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&crt);
        return (crt.QuadPart * 1000000) / freq.QuadPart;
    }

    HWND Win32Device::_CreateWindow(HINSTANCE hInstance)
    {
        const LPCSTR ClassName = __TEXT("Win32Device");

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

        const auto realWidth = clientSize.right - clientSize.left;
        const auto realHeight = clientSize.bottom - clientSize.top;

        int32_t windowLeft = 100;
        int32_t windowTop = 100;

        if (windowLeft < 0)
            windowLeft = 0;
        if (windowTop < 0)
            windowTop = 0;	// make sure window menus are in screen on creation

        // create window
        HWND hwnd = CreateWindow(ClassName, __TEXT("Shadow engine"), style, windowLeft, windowTop,
            realWidth, realHeight, NULL, NULL, hInstance, NULL);

        ShowWindow(hwnd, SW_SHOWNORMAL);
        UpdateWindow(hwnd);

        // fix ugly ATI driver bugs. Thanks to ariaci
        MoveWindow(hwnd, windowLeft, windowTop, realWidth, realHeight, TRUE);

        return hwnd;
    }
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, message, wParam, lParam);
}

