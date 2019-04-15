#include "platform/win32/Win32Device.h"
#include "video/RenderSystem.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto renderSystem = sh::video::RenderSystem::GetInstance();
    switch (message)
    {
    case WM_SIZE:
        {
            int32_t width = LOWORD(lParam);
            int32_t height = HIWORD(lParam);
            if (renderSystem)
                renderSystem->SetViewport(sh::math::Rect(0, 0, width, height));
        }
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    
}