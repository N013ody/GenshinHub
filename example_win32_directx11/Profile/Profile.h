
#pragma once
#include <wtypes.h>
#include <DX11Util.h>
#include <imgui.h>
#include <imgui_internal.h>


namespace Application {
    extern bool Init;  
    extern bool Exit;

}

namespace Procs {

    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HRESULT ResourceInit();

}

namespace Window {

    extern HWND BackendWindow;

    extern DirectXData WindowDatas;

    extern WNDCLASSEXW BackendWC;

}
