
#include <imgui.h>
#include <string>
#include <Profile.h>
#include "WindowUtil.h"
#include <dwmapi.h>

ImVec2 GetScreenCenter() {
    return ImVec2(Global::screenWidth / 2,Global::screenHeight / 2);
}

HWND GetWindowHWND(ImGuiWindow* window)
{
    return window->Viewport->PlatformHandleRaw ? (HWND)window->Viewport->PlatformHandleRaw : (HWND)window->Viewport->PlatformHandle;
}
ImGuiWindow* GetWindowByHwnd(HWND hwnd) {

    ImGuiContext& g = *GImGui;
    for (ImGuiWindow* window : g.Windows) {
        if (GetWindowHWND(window) == hwnd)return window;
    }
    return nullptr;
}

const WNDCLASSEXW GetWindowClass(const std::string& name) {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = Procs::WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = reinterpret_cast<LPCWSTR>(name.c_str());
    return wc;
}

int SetWindowStyles(HWND hwnd, DWORD styleVar) {
    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    if (exStyle == 0) {
        return exStyle;
    }

    //Ìí¼ÓÑùÊ½
    exStyle |= styleVar;


    LONG_PTR result = SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);
    if (result == 0) {
        return 0;
    }
   
    SetWindowPos(
        hwnd,
        HWND_TOPMOST,  
        0, 0, 0, 0,  
        SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_NOZORDER
    );
}

bool AreAllPlatformWindowsClosed() {
    ImGuiContext* context = ImGui::GetCurrentContext();
    if (context->Viewports.Size == 0) return true;

    for (ImGuiViewport* viewport : context->Viewports) {
        if (viewport->PlatformUserData != nullptr) {
            return false;
        }
    }
    return true;
}

