

#pragma comment(lib, "d3dcompiler.lib")

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
#include<Profile.h>
#include <WindowUtil.h>
#include <imgui.h>
#include <CallBackManager.h>
#include <DropManager.hpp>
#include <misc/freetype/imgui_freetype.h>
#include <GuiMain.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>



using namespace Window;
using namespace Global;



int main(int, char**){



    BackendWC=GetWindowClass("Pandora ImGui");

    RegisterClassExW(&BackendWC);

    BackendWindow = CreateWindowW(BackendWC.lpszClassName, L"Pandora ImGui Example", WS_POPUP, 100, 100, 500, 500, NULL, NULL, BackendWC.hInstance, NULL);

    ImGui::CreateContext();
    InitDirectX(BackendWindow, BackendWC);

    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    ::ShowWindow(BackendWindow, SW_HIDE);
    ::UpdateWindow(BackendWindow);

    IMGUI_CHECKVERSION();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;      
    io.ConfigViewportsNoAutoMerge = true;
    io.ConfigViewportsNoDecoration = true;
    io.ConfigViewportsNoDefaultParent = true;


    svgCacheManager.SetDevice(Window::WindowDatas.g_pd3dDevice);


    /**
    if (!lottieAnimator.Init("animation_test.json", Window::WindowDatas.g_pd3dDevice, 212, 212,false)) {
        return 0;
    }
    **/
    ImGuiContext& g_context = *GImGui;
    ImGuiIO g_io = ImGui::GetIO();
    (void)g_io;

    CallBackManager& callBackManager = CallBackManager::GetInstance(&g_context.PlatformIO);

    InitImGuiWithDragDrop();

    callBackManager.SetUpCallBack();


    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;

    float clearColor[4] = { 0.f, 0.f, 0.0f, 0.f };

    GuiMainInit();

    while (!Application::Exit)
    {

        MSG msg;
       
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
          
        }
     
        if (WindowDatas.g_SwapChainOccluded && (WindowDatas.g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED))
        {
            ::Sleep(10);
            continue;
        }
        WindowDatas.g_SwapChainOccluded = false;


        if (WindowDatas.g_ResizeWidth != 0 && WindowDatas.g_ResizeHeight != 0)
        {
            CleanupRenderTarget(WindowDatas.g_mainRenderTargetView);
            WindowDatas.g_pSwapChain->ResizeBuffers(0, WindowDatas.g_ResizeWidth, WindowDatas.g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            WindowDatas.g_ResizeWidth = WindowDatas.g_ResizeHeight = 0;
            CreateRenderTarget(WindowDatas.g_pd3dDevice, WindowDatas.g_pSwapChain,WindowDatas.g_mainRenderTargetView);
        }

     
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame(); {
            RenderMainWindow();
        }
        ImGui::Render();

      
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
           ImGui::UpdatePlatformWindows();
           ImGui::RenderPlatformWindowsDefault();
        }
        

         HRESULT hr = WindowDatas.g_pSwapChain->Present(1, 0);
         WindowDatas.g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

      }

      
END:
    Shutdown(BackendWindow,WindowDatas);
        
    return 0;
}





