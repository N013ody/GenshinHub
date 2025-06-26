
#include "InstallerWindow.h"
#include <ColorAnimator.h>

using namespace ImEasing;




bool InstallerWindow::button(ImVec2 size,float round, ImVec4 color_normal, ImVec4 color_hover, ImVec4 color_click, const char* label, const char* title, ImVec4 color_text)
{
    
    static std::unordered_map<ImGuiID, ColorAnimator> buttonAnimators;
    static const Params animParams{0.5f, 0.f, 1, false, Ease::Linear};

    ImGuiID id = ImGui::GetID(label);
    auto& animator = buttonAnimators.try_emplace(id, color_normal, animParams).first->second;

    bool result = ImGui::InvisibleButton(label, size);
    bool hovered = ImGui::IsItemHovered();
    bool active = ImGui::IsItemActive();

    animator.UpdateTarget(hovered ? (active ? color_click : color_hover)  : color_normal);
    animator.Tick();
    ImVec4 col = animator.Current();
    col.w = ImGui::GetStyle().Alpha;//不知道会不会明显干扰颜色过渡效果
    ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMin() + size, ImColor(col), round);
    
    if (title != nullptr) {
        ImVec2 textSize = ImGui::CalcTextSize(title);
        ImVec2 itemMin = ImGui::GetItemRectMin();
        const ImVec2 centerPos = ImVec2(
            itemMin.x + size.x * 0.5f - textSize.x * 0.5f,
            itemMin.y + size.y * 0.5f - textSize.y * 0.5f
        );
        ImGui::PushClipRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), true);
        color_text.w = ImGui::GetStyle().Alpha;
        ImGui::GetWindowDrawList()->AddText(centerPos, ImColor(color_text), title);
        ImGui::PopClipRect();
    }
    return result;
}

void InstallerWindow::tabBar(Page* pages, int pageCount, Page currentPage, ImVec2 size, const char* topBarName) {
    const Params widthParams{ 0.6f, 0.f, 1, false, Ease::OutCubic };
    const Params colorParams{ 0.3f, 0.f, 1, false, Ease::Linear };

    for (int i = 0; i < pageCount; i++) {
        Page page = pages[i];

       
        if (tabAnims.find(page) == tabAnims.end()) {
            TabIndicatorAnim anim;
            anim.targetActive = (page == currentPage);

            //宽度动画
            anim.widthAnim = std::make_shared<Animation<float>>();
            anim.widthAnim->Start(
                anim.targetActive ? 35.0f : 5.0f,
                anim.targetActive ? 35.0f : 5.0f,
                widthParams
            );


            //颜色动画
            anim.colorAnim = std::make_shared<Animation<ImVec4>>();
            anim.colorAnim->Start(
                anim.targetActive ? topbar_active : topbar_normal,
                anim.targetActive ? topbar_active : topbar_normal,
                colorParams
            );

            tabAnims[page] = anim;
        }

        //更新动画目标状态
        auto& anim = tabAnims[page];
        bool shouldBeActive = (page == currentPage);

        if (shouldBeActive != anim.targetActive) {
            anim.targetActive = shouldBeActive;

            //启动动画到新状态
            anim.widthAnim->Start(
                anim.widthAnim->GetCurrentValue(),
                shouldBeActive ? 35.0f : 5.0f,
                widthParams
            );

            //平滑过渡到目标颜色
            anim.colorAnim->Start(
                anim.colorAnim->GetCurrentValue(),
                shouldBeActive ? topbar_active : topbar_normal,
                colorParams
            );
        }

        //更新所有动画状态
        anim.widthAnim->Update();
      
        anim.colorAnim->Update();
    }

    //绘制
    ImGui::InvisibleButton(topBarName, size);
    ImGui::PushClipRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), true);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 originPos = ImGui::GetItemRectMin();
    const float spacing = 12.0f;

    originPos.y += size.y * 0.5f;
    const float leftPadding = 15.0f;
    originPos.x += leftPadding;

    for (int index = 0; index < pageCount; index++) {
        Page page = pages[index];
        auto& anim = tabAnims[page];

 
        const float currentWidth = anim.widthAnim->GetCurrentValue();

        ImVec4 currentColor = anim.colorAnim->GetCurrentValue();
        currentColor.w *= ImGui::GetStyle().Alpha; //应用全局透明度

 
        if (currentWidth > 2.5f) {
  
            drawList->AddRectFilled(
                ImVec2(originPos.x, originPos.y - 2.5f),
                ImVec2(originPos.x + currentWidth, originPos.y + 2.5f),
                ImColor(currentColor),
                2.5f
            );
            originPos.x += currentWidth + spacing;
        }
        else {
            //绘制圆点
            drawList->AddCircleFilled(
                ImVec2(originPos.x + 2.5f, originPos.y),
                2.5f,
                ImColor(currentColor)
            );
            originPos.x += 5.0f + spacing;
        }
    }

    ImGui::PopClipRect();
}

