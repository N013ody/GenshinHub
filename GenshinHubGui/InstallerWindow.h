
#define IMGUI_DEFINE_MATH_OPERATORS
#define WINDOW_INSTALLER "GenshinHub_Installer"
#define WINDOW_INSTALLER_CHILD "GenshinHub_Child"
#define INSTALLER_BACKGROUND_WIDTH 1042
#define INSTALLER_BACKGROUND_HEIGHT 732
#define INSTALLER_WIDTH 1030
#define INSTALLER_HEIGHT 720
#define INSTALLER_XOFFSET 4
#define INSTALLER_YOFFSET 4

#pragma once
#include <BaseWindow.h>
#include <imgui.h>
#include <WindowUtil.h>
#include <d3d11.h>
#include <ImageUtil.h>
#include <Images.h>
#include <ColorUtil.h>
#include <Profile.h>
#include <BaseWindow.h>
#include <EasingAnimationSystem.h>
#pragma comment(lib, "d3d11.lib")
using namespace ImEasing;

enum SwitchState {
    WAITING,
    LOADING,
    FINISH
};
enum Page {
    PAGE_WELCOME,
    PAGE_LOADING,
    PAGE_THEME,
    PAGE_LOADERSET,

};

struct TabIndicatorAnim {
    std::shared_ptr<Animation<float>> widthAnim;
    std::shared_ptr<Animation<ImVec4>> colorAnim;
    bool targetActive = false;
};

class InstallerWindow : public BaseWindow
{
    public:

        ID3D11ShaderResourceView* background = nullptr;
		ID3D11ShaderResourceView* logo = nullptr;
    
        InstallerWindow() {
			background = LoadTextureFromMemory(Window::WindowDatas.g_pd3dDevice, InstallerBackground, sizeof InstallerBackground);
			logo = LoadTextureFromMemory(Window::WindowDatas.g_pd3dDevice, Logo, sizeof Logo);

            this->setHeight(INSTALLER_BACKGROUND_HEIGHT);
            this->setWidth(INSTALLER_BACKGROUND_WIDTH);
            this->visble = true;
            this->init = false;
        }

        void draw() {

            ImGui::SetNextWindowSize(ImVec2(width,height));
       
            if (!init) {       
                ImGui::SetNextWindowPos(GetScreenCenter() - ImVec2(width / 2, height / 2));
                init = true;
            }

            ImGui::Begin(WINDOW_INSTALLER,this->visblePointer,STYLE_CUSTOM | ImGuiWindowFlags_NoScrollWithMouse);
            {

               ImGui::GetWindowDrawList()->AddImage(background,ImGui::GetWindowPos()+ImVec2(0,0), ImGui::GetWindowPos() + ImVec2(width,height));
               SetPosWithOffset(ImVec2(967,36));
               bool exit = button(ImVec2(20,20),30, button_exit_normal, button_exit_hover, button_exit_normal, "installer_exit");
               SetPosWithOffset(ImVec2(917, 36));
               bool minimize = button(ImVec2(20,20),30, button_minimize_normal, button_minimize_hover, button_minimize_normal,"installer_minimize");
               
               SetPosWithOffset(ImVec2(0,84));
               ImGui::BeginChild(WINDOW_INSTALLER_CHILD, ImVec2(INSTALLER_WIDTH, INSTALLER_HEIGHT),ImGuiChildFlags_None,ImGuiWindowFlags_NoScrollWithMouse); {
                 const ImVec2 centerPos = ImVec2(INSTALLER_WIDTH / 2, INSTALLER_HEIGHT / 2);
				 if (currentPage== PAGE_WELCOME) {

                     static float alpha_logo = 0.f;
                     static ImVec2 offset_image(0, 0);
                     static ImVec2 offset_text(0, 0);
                     static ImVec2 offset_button(0, 0);

                     static std::shared_ptr<Animation<ImVec2>> logo_PosAnim = std::make_shared<Animation<ImVec2>>();
                     static std::shared_ptr<Animation<ImVec2>> button_PosAnim = std::make_shared<Animation<ImVec2>>();
                     static std::shared_ptr<Animation<ImVec2>> text_PosAnim = std::make_shared<Animation<ImVec2>>();

                     static std::shared_ptr<Animation<float>> logo_AlphaAnim = std::make_shared<Animation<float>>();
                     static std::shared_ptr<Animation<float>> button_AlphaAnim = std::make_shared<Animation<float>>();
                     static std::shared_ptr<Animation<float>> text_AlphaAnim = std::make_shared<Animation<float>>();

                     static AnimationController welcome_controller;

                     if (!initPage) {
                         offset_image= ImVec2(50, 0);
                         offset_text = ImVec2(60, 0);
                         offset_button = ImVec2(100, 0);
                         alpha_logo = 0.f;
                    
                         logo_PosAnim->AddTrigger(0.1f, [] {
                             Params params;
                             params.duration = 1.f;
                             params.easing = Ease::InOutCirc;
                             params.pingPong = false;
                             logo_AlphaAnim->Start(
                                 0.f,
                                 1.f,
                                 params
                             );
                             });
                         logo_AlphaAnim->AddTrigger(0.2f, [] {
                             Params params;
                             params.duration = 1.3f;
                             params.easing = Ease::InOutCirc;
                             params.pingPong = false;
                             text_PosAnim->Start(
                                 ImVec2(0.f,0.f),
                                 ImVec2( -60.f,0.F),
                                 params
                             );
                         });
                         text_PosAnim->AddTrigger(0.1f, [] {
                             Params params;
                             params.duration = 1.f;
                             params.easing = Ease::InOutCirc;
                             params.pingPong = false;
                             text_AlphaAnim->Start(
                                 0.f,
                                 1.f,
                                 params
                             );
                             });
                         text_AlphaAnim->AddTrigger(0.2f, [] {
                             Params params;
                             params.duration = 1.3f;
                             params.easing = Ease::InOutCirc;
                             params.pingPong = false;
                             button_PosAnim->Start(
                                 ImVec2(0.f, 0.f),
                                 ImVec2(-100.f, 0.F),
                                 params
                             );
                             });
                         button_PosAnim->AddTrigger(0.1f, [] {
                             Params params;
                             params.duration = 1.f;
                             params.easing = Ease::InOutCirc;
                             params.pingPong = false;
                             button_AlphaAnim->Start(
                                 0.f,
                                 1.f,
                                 params
                             );
                             });
                         welcome_controller.AddAnimation(logo_PosAnim);
                         welcome_controller.AddAnimation(logo_AlphaAnim);
                         welcome_controller.AddAnimation(text_PosAnim);
                         welcome_controller.AddAnimation(text_AlphaAnim);
                         welcome_controller.AddAnimation(button_PosAnim);
                         welcome_controller.AddAnimation(button_AlphaAnim);
                         logo_PosAnim->Start(ImVec2(0, 0), ImVec2(-50, 0), Params{ 1.3f,0.f,1,false, Ease::InOutCirc });

                         initPage = true;
                     }
                     logo_PosAnim->Update();
                     welcome_controller.UpdateAll();

                     ImGui::PushStyleVar(ImGuiStyleVar_Alpha, logo_AlphaAnim->GetCurrentValue());
                     ImGui::SetCursorPos(ImVec2(502, 105)+offset_image + logo_PosAnim->GetCurrentValue());
					 ImGui::Image(logo, ImVec2(50, 50));
                     ImGui::PopStyleVar();


                     ImGui::PushStyleVar(ImGuiStyleVar_Alpha, button_AlphaAnim->GetCurrentValue());
                     ImGui::SetCursorPos(ImVec2(421, 399) + offset_button + button_PosAnim->GetCurrentValue());
					 bool start=button(ImVec2(230, 40), 5, button_start_normal, button_start_hover, button_start_active, "button_start", "Start", text_col);
                     ImGui::PopStyleVar();

                     ImGui::PushStyleVar(ImGuiStyleVar_Alpha, text_AlphaAnim->GetCurrentValue());
                     ImGui::SetCursorPos(centerPos-ImGui::CalcTextSize("Genshin Hub")/2 + offset_text + text_PosAnim->GetCurrentValue() - ImVec2(-10, 100));
                     ImGui::Text("Genshin Hub");
                     ImGui::SetCursorPos(centerPos - ImGui::CalcTextSize("help you get more and more mod management easy") / 2 + offset_text + text_PosAnim->GetCurrentValue() - ImVec2(-10, 70));
                     ImGui::PushStyleColor(ImGuiCol_Text, text_subtitle_col);
                     ImGui::Text("help you get more and more mod management easy");
                     ImGui::PopStyleColor();
                     ImGui::PopStyleVar();

                     if (start) 
                         SwitchPage(PAGE_LOADING);
                     
				 }
                 else if (currentPage == PAGE_LOADING) {

                 }

                 PageUpdate();
                 ImVec2 tabBarSize(110,32);
				 ImGui::SetCursorPos(ImVec2(INSTALLER_WIDTH/2-30,INSTALLER_HEIGHT-140));
				 tabBar(pages,4,currentPage,tabBarSize, "installer_tabbar");
               }ImGui::EndChild();
               

               if (exit) {
                   visble = false;
                   Application::Exit = true;
               }
               if (minimize) {
                   ShowWindow((HWND)ImGui::GetWindowViewport()->PlatformHandle, SW_SHOWMINIMIZED);
               }


            }ImGui::End();
        }

    private:

        Page nextPage = Page::PAGE_WELCOME;
		Page currentPage= PAGE_WELCOME;
		Page pages[4] = {PAGE_WELCOME,PAGE_LOADING,PAGE_THEME,PAGE_LOADERSET};

        SwitchState currentState=SwitchState::WAITING;

        /*
        *当前Page是否初始化
        */
        bool initPage=false;

        /*
        * tabbar动画管理集合
        */
        std::unordered_map<Page, TabIndicatorAnim> tabAnims;

        /*
        * All Colors 所有颜色
        */
        const ImVec4 button_exit_normal = HexToImVec4("#FF7C7E",1.f);
        const ImVec4 button_exit_hover = HexToImVec4("#FF5B5E", 1.f);
        const ImVec4 button_exit_active = HexToImVec4("#F11014", 1.f);
        const ImVec4 text_col = HexToImVec4("#FFFFFF", 1.f);
        const ImVec4 text_title_col = HexToImVec4("#322B2B", 1.f);
        const ImVec4 text_subtitle_col = HexToImVec4("#827D7D", 1.f);

        const ImVec4 button_minimize_normal = HexToImVec4("#9D8E8E", 1.f);
        const ImVec4 button_minimize_hover = HexToImVec4("#B0A4A4", 1.f);
        const ImVec4 button_minimize_active = HexToImVec4("#7A6C6C", 1.f);

		const ImVec4 button_start_normal = HexToImVec4("#CBBCF8", 1.f);
		const ImVec4 button_start_hover = HexToImVec4("#D6C9FE", 1.f);
		const ImVec4 button_start_active = HexToImVec4("#BEABF5", 1.f);

		const ImVec4 topbar_active = HexToImVec4("#B29EE0", 1.f);
		const ImVec4 topbar_normal = HexToImVec4("#CBBCF8", 1.f);


        bool button(ImVec2 size, float round, ImVec4 color_normal, ImVec4 color_hover, ImVec4 color_click, const char* label, const char* title = nullptr, ImVec4 color_text = ImVec4(255,255,255,1.f));
            
		void tabBar(Page* pages, int pageCount, Page currentPage,ImVec2 size, const char* topBarName = nullptr);


        void SetPosWithOffset(ImVec2 pos = ImVec2(0, 0)) {
            ImGui::SetCursorPos(ImVec2(INSTALLER_XOFFSET, INSTALLER_YOFFSET) + pos);
        }

        void SwitchPage(Page page) {
            if (currentPage == page) return;
            nextPage = page;
        }

        void PageUpdate() {
            if (nextPage == currentPage)return;
            if (currentState == SwitchState::LOADING)return;

            currentState = SwitchState::WAITING;
            currentPage = nextPage;
        }

	
};

