#define IMGUI_DEFINE_MATH_OPERATORS
#include <GuiMain.h>
#include <pimgui.h>
#include <imgui.h>
#include<Profile.h>
#include <string>
#include <EasingAnimationSystem.h>
#include <ImageUtil.h>
#include <InstallerWindow.h>
#include <Images.h>

using namespace ImEasing;
using namespace Global;
using namespace Window;




HRESULT GuiMainInit() {


    ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    style.FramePadding = { 0,0 };

   // install_window.background = LoadTextureFromMemory(Window::WindowDatas.g_pd3dDevice, InstallerBackground, sizeof InstallerBackground);

    return S_OK;
}

void RenderMainWindow() {
    static InstallerWindow install_window = InstallerWindow();
    install_window.draw();
    if (!install_window.isVisble())
        Application::Exit = true;


}
