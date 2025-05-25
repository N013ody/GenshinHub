#define IMGUI_DEFINE_MATH_OPERATORS
#include <GuiMain.h>
#include <pimgui.h>
#include <imgui.h>
#include<Profile.h>
#include <string>
#include <EasingAnimationSystem.h>


HRESULT GuiMainInit() {
    return D3DX11CreateShaderResourceViewFromMemory(Window::WindowDatas.g_pd3dDevice, TestPlayImage, sizeof(TestPlayImage), nullptr, nullptr, &playImagePtr, 0);
}

static bool show_test_window = true;
static bool show_dropfile_window = true;


float DemoFunction(float x) {
    return sin(x * 2.0f) * exp(-x * 0.2f);
}


void RenderMainWindow() {


    static float f = 0.0f;
    static int counter = 0;
    static bool init = false;
    static char inputText[256] = { 0 };
    static char inputTextMulti[256] = { 0 };

    ImGui::Begin("Pandora ImGui",&show_test_window,ImGuiWindowFlags_BlurBackGround|ImGuiWindowFlags_ShadowWindow);
    ImGui::InputText("Input Popup Test ", inputText, sizeof inputText, ImGuiInputTextFlags_ClickList);
    ImGui::InputTextMultiline("AnimInput Multiline Test!", inputTextMulti, sizeof inputTextMulti,ImVec2(0,0), ImGuiInputTextFlags_ClickList);


    ImGui::End();

  
  
    ImGui::Begin("Hello, world!", &show_dropfile_window, ImGuiWindowFlags_DropFiles);{


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


    std::vector<std::string>files = PImGui::GetCurrentWindowFiles();
    for (const std::string file : files) {
        ImGui::Text("files = %s", file.c_str());
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


    ImGui::Text("Function Plot Test");
    //Function example
    static FunctionPlotConfig cfg;
    cfg.line_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // ÂÌÉ«ÇúÏß
    cfg.grid_spacing = 2.0f;
    cfg.line_thickness = 1.f;
    cfg.grid_thickness = 0.5f;
    cfg.axis_thickness = 1.f;

    PImGui::FunctionPlot("Wave", ImEasing::Ease::InOutQuad, -5.0f, 5.0f, ImVec2(100, 100), &cfg,PImGuiFunctinPlotFlags_NoGrid);

    
    ImGui::Text("Svg Test");
    //Svg example
    static float scale = 1.0f;
    ImGui::SliderFloat("Scale", &scale, 0.5f, 2.0f);


    static SVGTexture tex{};
    if (!tex.srv)
        tex = CreateSVGTexture(Window::WindowDatas.g_pd3dDevice, "testsvg.svg");

    if (tex.srv)
    {
        
        ImGui::Image(tex.srv, tex.size); 
 
    }
    
    }ImGui::End();
    
    if (!show_dropfile_window && !show_test_window)Application::Exit = true;


}
