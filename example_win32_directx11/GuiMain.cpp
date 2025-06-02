#define IMGUI_DEFINE_MATH_OPERATORS
#include <GuiMain.h>
#include <pimgui.h>
#include <imgui.h>
#include<Profile.h>
#include <string>
#include <EasingAnimationSystem.h>
#include <WICTextureLoader.h>
#include <Shaders.h>
using namespace ImEasing;
using namespace DirectX;
using namespace Global;
using namespace Window;

HRESULT GuiMainInit() {
    return CreateWICTextureFromMemory(
        Window::WindowDatas.g_pd3dDevice,      
        nullptr,                               
        reinterpret_cast<const uint8_t*>(TestPlayImage),
        sizeof(TestPlayImage),                 
        &textureResource,                     
        &playImagePtr,                         
        0                                      
    );
}

static bool show_test_window = true;
static bool show_dropfile_window = true;


float DemoFunction(float x) {
    return sin(x * 2.0f) * exp(-x * 0.2f);
}



void RenderBasicAnimations() {
    static auto floatAnim = std::make_shared<Animation<float>>();

    static float floatValue = 0.0f;

    static auto vec2Anim = std::make_shared<Animation<ImVec2>>();

    static auto colorAnim = std::make_shared<Animation<ImVec4>>();

    static ImVec4 bgColor = { 0.2f, 0.2f, 0.8f, 1.0f };

    static float duration=1.f;
    static float delay=0.f;
    static int current_item = 0;
    static std::function<float(float)> current_easing = Ease::Linear;

    const char* easing_items[] = {
     "Linear",
     "InQuad", "OutQuad", "InOutQuad",
     "InCubic", "OutCubic", "InOutCubic",
     "InQuart", "OutQuart", "InOutQuart",
     "InSine", "OutSine", "InOutSine",
     "InExpo", "OutExpo", "InOutExpo",
     "InCirc", "OutCirc", "InOutCirc",
     "InElastic", "OutElastic", "InOutElastic",
     "InBounce", "OutBounce", "InOutBounce"
    };


    static const std::function<float(float)> easing_funcs[] = {
        Ease::Linear,
        Ease::InQuad, Ease::OutQuad, Ease::InOutQuad,
        Ease::InCubic, Ease::OutCubic, Ease::InOutCubic,
        Ease::InQuart, Ease::OutQuart, Ease::InOutQuart,
        Ease::InSine, Ease::OutSine, Ease::InOutSine,
        Ease::InExpo, Ease::OutExpo, Ease::InOutExpo,
        Ease::InCirc, Ease::OutCirc, Ease::InOutCirc,
        Ease::InElastic, Ease::OutElastic, Ease::InOutElastic,
        Ease::InBounce, Ease::OutBounce, Ease::InOutBounce
    };

    ImGui::SliderFloat("animation duration", &duration, 0.0f, 10.0f);
    ImGui::SliderFloat("animation delay",&delay,0.0f,10.0f);

    ImGui::Combo("easing function", &current_item, easing_items, IM_ARRAYSIZE(easing_items));
    current_easing = easing_funcs[current_item];

    ImGui::Text("Float Anim:");
    if (ImGui::Button("Start Anim")) {
        Params params;
        params.duration = duration;
        params.delay = delay;
        params.easing = current_easing;
        floatAnim->Start(0.0f, 1.0f, params);
    }

    ImGui::SameLine();
    floatAnim->Update();
    ImGui::ProgressBar(floatAnim->GetCurrentValue(), ImVec2(200, 20));

    static ImVec2 position;
    ImGui::Text("Pos Animation:");
    if (ImGui::Button("Move Rect")) {
        Params params;
        params.duration = duration;
        params.delay = delay;
        params.easing = current_easing;
        position = ImGui::GetItemRectMin()+ImVec2(100,0);
        vec2Anim->Start(position, ImVec2(position.x + 200, position.y ), params);
    }

    vec2Anim->Update();
    ImVec2 positio = vec2Anim->GetCurrentValue();
    ImGui::GetWindowDrawList()->AddRectFilled(
        positio,
        ImVec2(positio.x + 50, positio.y + 50),
        IM_COL32(255, 100, 100, 255)
    );

   
    ImGui::Text("Color Animation:");

    if (ImGui::Button("Color Change")) {
        Params params;
        params.duration = duration;
        params.delay = delay;
        params.easing = current_easing;
        colorAnim->Start(
            bgColor,
            ImVec4(0.8f, 0.2f, 0.2f, 1.0f),
            params
        );
    }
    colorAnim->Update();
    if (colorAnim->GetState()== ImEasing::Running) {
        bgColor = colorAnim->GetCurrentValue();
    }
    ImGui::SetNextItemWidth(150);
    ImGui::ColorPicker4("###bg_color", &bgColor.x, ImGuiColorEditFlags_None);
    ImGui::ColorButton("##bg", bgColor, ImGuiColorEditFlags_None, ImVec2(200, 30));

}


void RenderCallbackDemo() {
    static auto anim = std::make_shared<Animation<float>>();
    static float value = 0.0f;
    static std::vector<float> history;

    //创建回调
    auto callback = std::make_shared<TypedCallback<float>>(
        [&](const float& val, float t) {
            value = val;
            if (history.size() > 50) history.erase(history.begin());
            history.push_back(val);
        }
    );
    anim->SetCallback(callback);

    ImGui::Text("CallBack Example:");
    if (ImGui::Button("Start Animation")) {
        Params params;
        params.duration = 4.0f;
        params.easing = Ease::InOutBounce;
        anim->Start(0.0f, 1.0f, params);
    }
    anim->Update();
   
    ImGui::PlotLines("Animation history data:", history.data(), history.size(), 0, NULL, 0.0f, 1.0f, ImVec2(300, 80));
}


void RenderTriggerDemo() {
    static auto anim = std::make_shared<Animation<float>>();
    static int triggerCount = 0;
    static bool trigger = false;
    static bool triggerz = false;
    anim->AddTrigger(0.3f, [] { trigger = true; });
    anim->AddTrigger(0.7f, [] { triggerz = true; });
    anim->AddTrigger(1.0f, [&] { triggerCount++; });

    ImGui::Text("Trigger Example:");

    if (ImGui::Button("Play Animation")) {
        Params params;
        params.duration = 3.0f;
        anim->Start(0.0f, 1.0f, params);
    }
    ImGui::SameLine();
    ImGui::Text("Trigger count: %d", triggerCount);
    if (trigger)ImGui::Text("Trigger 30%");
    if (triggerz)ImGui::Text("Trigger 70%");
    anim->Update();
    ImGui::ProgressBar(anim->GetCurrentValue(), ImVec2(300, 20));
}


void RenderLoopDemo() {
    static auto anim = std::make_shared<Animation<float>>();
    static float angle = 0.0f;
    static bool pingpong= false;
    static int loopcount = 1;
    ImGui::Checkbox("Pingpong loop",&pingpong);
    ImGui::SliderInt("Loop count", &loopcount, 1, 10);

    ImGui::Text("Loop Animation:");
    if (ImGui::Button("Start normal animation")) {
        Params params;
        params.duration = 1.0f;
        params.loopCount = loopcount; //循环5次
        params.pingPong = pingpong;
        anim->Start(0.0f, 1.0f, params);
    }




    anim->Update();
    angle = anim->GetCurrentValue() * IM_PI * 2;
    ImVec2 center = ImGui::GetCursorScreenPos();

    center.x += 100;
    center.y += 50;
    ImGui::SetCursorPos(center);
    ImGui::InvisibleButton("###invisble_item_circle", ImVec2(200, 200));
    ImGui::GetWindowDrawList()->AddCircle(
        center,
        40,
        IM_COL32(100, 200, 100, 255),
        0, 3.0f
    );
    ImGui::GetWindowDrawList()->AddLine(
        center,
        ImVec2(center.x + cosf(angle) * 40, center.y + sinf(angle) * 40),
        IM_COL32(255, 255, 255, 255),
        2.0f
    );
}


void RenderCompositeDemo() {
    static auto posAnim = std::make_shared<Animation<ImVec2>>();
    static auto sizeAnim = std::make_shared<Animation<float>>();
    static auto colorAnim = std::make_shared<Animation<ImVec4>>();
    
    static ImVec2 position = ImGui::GetCursorScreenPos();
    static float size = 30.0f;
    static ImVec4 color = { 0.8f, 0.2f, 0.8f, 1.0f };

    ImGui::Text("Composite Animation:");
    if (ImGui::Button("Start Composite Animation")) {
 
        //位置动画
        Params posParams;
        posParams.duration = 2.0f;
        posParams.easing = [](float t) {
            return t < 0.5 ? 4 * t * t * t : 1 - powf(-2 * t + 2, 3) / 2;
            };
        posAnim->Start(position, ImVec2(450, 300), posParams);

        //大小动画
        Params sizeParams;
        sizeParams.duration = 2.0f;
        sizeParams.delay = 0.3f;
        sizeAnim->Start(30.0f, 80.0f, sizeParams);

        //颜色动画
        Params colorParams;
        colorParams.duration = 2.0f;
        colorParams.pingPong = true;
        colorParams.loopCount = 2;
        colorAnim->Start(
            ImVec4(0.8f, 0.2f, 0.8f, 1.0f),
            ImVec4(0.2f, 0.8f, 0.5f, 0.7f),
            colorParams
        );
    }

    posAnim->Update();
    sizeAnim->Update();
    colorAnim->Update();
    position = posAnim->GetCurrentValue();
    size = sizeAnim->GetCurrentValue();
    color = colorAnim->GetCurrentValue();

    ImU32 col32 = ImColor(color);

    ImGui::GetWindowDrawList()->AddCircleFilled(
        position,
        size,
        col32
    );
}

static AnimationController m_controller;

static ImVec2 g_BoxPosition = ImVec2(0.0f, 0.0f);
static ImVec4 g_BoxColor = ImVec4(0.2f, 0.2f, 0.8f, 1.0f);
static ImVec2 g_BoxSize = ImVec2(100.0f, 100.0f);

static std::shared_ptr<Animation<ImVec4>> g_ColorAnim = std::make_shared<Animation<ImVec4>>();
static std::shared_ptr<Animation<ImVec2>> g_PosAnim = std::make_shared<Animation<ImVec2>>();
static std::shared_ptr<Animation<float>> g_ScaleAnim = std::make_shared<Animation<float>>();


static bool initialized = false;


void InitAnimationChain() {

    g_ColorAnim->AddTrigger(0.1f, [] {
        Params params;
        params.duration = 1.5f;
        params.easing = Ease::InOutCirc;
        params.pingPong = true;
        g_PosAnim->Start(
            ImVec2(100.0f, 100.0f),
            ImVec2(500.0f, 300.0f),
            params
        );
        });

    //位移动画一半时触发缩放动画
    g_PosAnim->AddTrigger(0.5f, [] {
        Params params;
        params.duration = 1.0f;
        params.easing = Ease::InQuad;
        params.loopCount = 1;
        params.pingPong = false;
        g_ScaleAnim->Start(1.0f, 2.0f, params);
        });

    //注册
    m_controller.AddAnimation(g_ColorAnim);
    m_controller.AddAnimation(g_PosAnim);
    m_controller.AddAnimation(g_ScaleAnim);
    initialized = true;

}
void RenderChainDemo() {

    if (!initialized) InitAnimationChain();

    m_controller.UpdateAll();

    g_BoxColor = g_ColorAnim->GetCurrentValue();
     g_BoxPosition = g_PosAnim->GetCurrentValue();
     static float scale = 1;
    if (g_ScaleAnim->GetState() == ImEasing::Running) 
    scale=g_ScaleAnim->GetCurrentValue();

    ImVec2 scaledSize(g_BoxSize.x * scale, g_BoxSize.y * scale);


    if (ImGui::Button("Start Chain Animation")) {
        Params params;
        params.duration = 2.0f;
        params.easing = Ease::InCubic;
        g_ColorAnim->Start(
            ImVec4(0.2f, 0.2f, 0.8f, 1.0f),
            ImVec4(0.8f, 0.2f, 0.2f, 1.0f),
            params
        );
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    drawList->AddRectFilled(
        ImGui::GetCurrentWindow()->Pos + g_BoxPosition,
        ImVec2(ImGui::GetCurrentWindow()->Pos.x+g_BoxPosition.x + scaledSize.x,
            ImGui::GetCurrentWindow()->Pos.y+g_BoxPosition.y + scaledSize.y),
        ImColor(g_BoxColor)
    );

    ImGui::Text("Position: (%.1f, %.1f)", g_BoxPosition.x, g_BoxPosition.y);
    ImGui::Text("Scale: %.2f", g_ScaleAnim->GetCurrentValue());


}

void RenderMainWindow() {


    static float f = 0.0f;
    static int counter = 0;
    static bool init = false;
    static char inputText[256] = { 0 };
    static char inputTextMulti[256] = { 0 };


    ImGui::Begin("Pandora ImGui Animation Test",&show_test_window,ImGuiWindowFlags_BlurBackGround|ImGuiWindowFlags_ShadowWindow);

    ImGui::InputText("Input Popup Test ", inputText, sizeof inputText, ImGuiInputTextFlags_ClickList);
    ImGui::InputTextMultiline("AnimInput Multiline Test!", inputTextMulti, sizeof inputTextMulti,ImVec2(0,0), ImGuiInputTextFlags_ClickList);

    if (ImGui::CollapsingHeader("Basic Animations")) {
        RenderBasicAnimations();
    }
    if (ImGui::CollapsingHeader("Callback Demo")) {
        RenderCallbackDemo();
    }
    if (ImGui::CollapsingHeader("Trigger Demo")) {
        RenderTriggerDemo();
    }
    if (ImGui::CollapsingHeader("Loop Demo")) {
        RenderLoopDemo();
    }
    if (ImGui::CollapsingHeader("CompositeDem")) {
        RenderCompositeDemo();
    }
    if (ImGui::CollapsingHeader("Chain Demo")) {
        RenderChainDemo();
    }

   // m_controller.UpdateAll();


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

    ImGui::Text("This is some useful text.");            

    ImGui::SameLine();


    std::vector<std::string>files = PImGui::GetCurrentWindowFiles();
    for (const std::string file : files) {
        ImGui::Text("files = %s", file.c_str());
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


    ImGui::Text("Function Plot Test");
    //Function example
    static FunctionPlotConfig cfg;
    cfg.line_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); 
    cfg.grid_spacing = 2.0f;
    cfg.line_thickness = 1.f;
    cfg.grid_thickness = 0.5f;
    cfg.axis_thickness = 1.f;

    PImGui::FunctionPlot("Wave", ImEasing::Ease::InOutQuad, -5.0f, 5.0f, ImVec2(100, 100), &cfg,PImGuiFunctinPlotFlags_NoGrid);

    
    ImGui::Text("SVG Test");
    //Svg example
    static float scale = 1.0f;
    ImGui::SliderFloat("Scale", &scale, 0.5f, 2.0f);

    SVGTexture texture = Global::cacheManager.GetScaledTexture(scale);

    static SVGTexture texture_svg = CreateSVGTexture(Window::WindowDatas.g_pd3dDevice,"testsvg.svg",1.f,true);
    static NSVGimage* svgImage = nsvgParseFromFile("testsvg.svg", "px", 96.0f);
    static SVGTexture static_texture_svg = CreateSVGTexture(Window::WindowDatas.g_pd3dDevice, svgImage, 1.f, true);

    // 渲染时使用
    ImGui::Image(texture.srv, texture.size);
    ImGui::SameLine();
    ImGui::Image(texture_svg.srv, texture_svg.size);
    ImGui::SameLine();
    ImGui::Image(static_texture_svg.srv, static_texture_svg.size);

    if (ImGui::Button("Start")) {
        Global::g_animator.Play();
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
        Global::g_animator.Stop();
    }

    //更新动画
    static float lastTime = 0.0f;
    float currentTime = (float)GetTickCount() / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    Global::g_animator.Update(deltaTime);

    ImGui::Image(Global::g_animator.GetTextureID(),Global::g_animator.GetSize());
    }ImGui::End();

    if (!show_dropfile_window && !show_test_window)Application::Exit = true;


}
