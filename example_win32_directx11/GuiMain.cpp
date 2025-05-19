#define IMGUI_DEFINE_MATH_OPERATORS
#include <GuiMain.h>
#include <imgui.h>
#include<Profile.h>

HRESULT GuiMainInit() {
    return D3DX11CreateShaderResourceViewFromMemory(Window::WindowDatas.g_pd3dDevice, TestPlayImage, sizeof(TestPlayImage), nullptr, nullptr, &playImagePtr, 0);
}

void RenderMainWindow() {

    
    static float f = 0.0f;
    static int counter = 0;
    static bool init = false;
    static char inputText[256] = { 0 };
    static char inputTextMulti[256] = { 0 };

    ImGui::Begin("Pandora ImGui", NULL, ImGuiWindowFlags_ShadowWindow|ImGuiWindowFlags_BlurBackGround);
    ImGui::InputText("AnimInput Test", inputText, sizeof inputText, ImGuiInputTextFlags_AnimScroll);
    ImGui::InputTextMultiline("AnimInput Multiline Test!", inputTextMulti, sizeof inputTextMulti);
    if (ImGui::Button("####EXITBUTTON", ImVec2(100, 50)))Application::Exit = true;
    ImGui::End();
    /**
    ImGui::Begin("Pandora ImGui Test", NULL, ImGuiWindowFlags_ShadowWindow | ImGuiWindowFlags_BlurBackGround);
    ImGui::InputText("AnimInput Test", inputText, sizeof inputText, ImGuiInputTextFlags_AnimScroll);
    ImGui::InputTextMultiline("AnimInput Multiline Test!", inputTextMulti, sizeof inputTextMulti);
    if (ImGui::Button("####EXITBUTTON", ImVec2(100, 50)))Application::Exit = true;
    ImGui::End();
    **/
  
    ImGui::Begin("Hello, world!", NULL, ImGuiWindowFlags_DropFiles);{


    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();

    const ImU32 shadow_color = IM_COL32(0, 0, 0, 50);
    const float shadow_size = 4.0f;
    ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(
        window_pos + ImVec2(0, window_size.y),
        window_pos + window_size + ImVec2(0, shadow_size),
        shadow_color, shadow_color, IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0)
    );

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

    ImGui::SameLine();
    /**
    ImGui::Text("counter = %d", counter);
    HWND currentHwnd = GetWindowHWND(ImGui::GetCurrentWindow());
    if (DropFile* dropFile = GetDropFileByHwnd(currentHwnd)) {
        std::vector<std::string>files = dropFile->files;
        for (const std::string file : files) {
            ImGui::Text("files = %s", file.c_str());
        }
    }
    **/


    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);



    ImVec2 pos = ImGui::GetWindowPos();


    ImGui::Button("Buttonxx", ImVec2(100, 100));



    }ImGui::End();


}
