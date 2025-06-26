#include "Profile.h"


using namespace Window;

LottieAnimator Global::lottieAnimator;

SVGCacheManager Global::svgCacheManager;

bool Application::isInitialized = false;

bool Application::Exit = false;

HWND Window::BackendWindow=nullptr;

DirectXData Window::WindowDatas = {}; WNDCLASSEXW Window::BackendWC = {};


int Global::screenWidth = GetSystemMetrics(SM_CXSCREEN);
int Global::screenHeight = GetSystemMetrics(SM_CYSCREEN);



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI Procs::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        WindowDatas.g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        WindowDatas.g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

