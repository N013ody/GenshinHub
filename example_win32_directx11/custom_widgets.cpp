#define IMGUI_DEFINE_MATH_OPERATORS

#include <pimgui.h>
using namespace ImGui;

bool PImGui::InputTextPopup(const char* str_id, char* buffer, PImGuiTextPopupFlags popup_flags) {
    bool is_open = ImGui::BeginPopup(str_id);
    if (!is_open) return false;

    //焦点设置
    if (ImGui::IsWindowAppearing())
        ImGui::SetKeyboardFocusHere();

    ImGui::BeginTable("##Buttons", 2);
    if (ImGui::TableNextColumn()) {
        if (ImGui::Button("Copy")) {
            ImGui::SetClipboardText(buffer);
        }
        if (!(popup_flags & PImGuiTextPopupFlags_NoPaste)) {
            bool paste = ImGui::Button("Paste");
            const char* clipboard = ImGui::GetClipboardText();
            if (!(popup_flags & PImGuiTextPopupFlags_NoPaste)) {
                bool paste = ImGui::Button("Paste");
                const char* clipboard = ImGui::GetClipboardText();
                if (clipboard && paste) {
                    size_t clipboard_len = strlen(clipboard);
                    size_t buffer_size = sizeof(buffer);
                    strncpy(buffer, clipboard, buffer_size - 1);
                    buffer[buffer_size - 1] = '\0';

                }

            }
        }
        if (ImGui::TableNextColumn()) {
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 0, 0, 255));
            if (ImGui::Button("Exit")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor();
        }
        ImGui::EndTable();

        ImGui::EndPopup();
        return true;
    }
};

#include <algorithm>

bool PImGui::FunctionPlot(const char* label,
    float (*func)(float x),
    float x_min,
    float x_max,
    const ImVec2& size,
    const FunctionPlotConfig* config,
    PImGuiFunctinPlotFlags flags)
{
    // 获取当前窗口上下文
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems || !func)
        return false;

    // 初始化配置参数
    const FunctionPlotConfig cfg = config ? *config : FunctionPlotConfig();

    // 获取唯一标识符
    const ImGuiID id = window->GetID(label);
    const ImVec2 frame_size = CalcItemSize(size, 256.0f, 256.0f); // 设置默认最小尺寸
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);


    ItemSize(frame_size);
    if (!ItemAdd(frame_bb, id))
        return false;

   
    const bool is_hovered = ItemHoverable(frame_bb, id, ImGuiItemFlags_None);
    bool is_clicked = false;

    //坐标变换参数
    static ImVec2 offset(0.0f, 0.0f);
    static float scale = 1.0f;
    scale = std::clamp(scale, 0.5f, 10.0f); //限制缩放范围

  
    auto ToScreen = [&](float x, float y) -> ImVec2 {
        // 输入值约束
        x = std::clamp(x, x_min, x_max);
        y = std::clamp(y, -1.0f, 1.0f);

        return ImVec2(
            ((x - x_min) / (x_max - x_min)) * frame_size.x * scale + frame_bb.Min.x + offset.x,
            frame_bb.Max.y - ((y + 1.0f) / 2.0f) * frame_size.y * scale + offset.y
        );
        };

    // 获取绘制列表并设置裁剪区域
    ImDrawList* draw_list = GetWindowDrawList();
    draw_list->PushClipRect(frame_bb.Min, frame_bb.Max, true); 

  
    draw_list->AddRectFilled(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg));


    const float base_samples = frame_size.x * scale;
    const int samples = static_cast<int>(std::clamp(base_samples, 100.0f, 2000.0f)); 


    if (!(flags& PImGuiFunctinPlotFlags_NoGrid)) {
        const float grid_step_x = (x_max - x_min) / 10.0f;
        const float grid_step_y = 0.2f;

        //纵向网格线
        for (float x = x_min; x <= x_max; x += grid_step_x) {
            ImVec2 p0 = ToScreen(x, -1.0f);
            ImVec2 p1 = ToScreen(x, 1.0f);
            draw_list->AddLine(p0, p1, GetColorU32(ImGuiCol_Border), cfg.grid_thickness);
        }

        //横向网格线
        for (float y = -1.0f; y <= 1.0f; y += grid_step_y) {
            ImVec2 p0 = ToScreen(x_min, y);
            ImVec2 p1 = ToScreen(x_max, y);
            draw_list->AddLine(p0, p1, GetColorU32(ImGuiCol_Border), cfg.grid_thickness);
        }
    }


    ImVec2 prev_point;
    bool first_point = true;
    for (int i = 0; i < samples; ++i) {
        const float t = static_cast<float>(i) / (samples - 1);
        const float x = x_min + t * (x_max - x_min);
        const float y = func(x);

        const ImVec2 p = ToScreen(x, y);

        if (!first_point) {
            //检查线段是否在可视区域内
            const bool line_visible =
                (prev_point.x >= frame_bb.Min.x && prev_point.x <= frame_bb.Max.x) ||
                (p.x >= frame_bb.Min.x && p.x <= frame_bb.Max.x);

            if (line_visible) {
                draw_list->AddLine(
                    prev_point, p,
                    ColorConvertFloat4ToU32(cfg.line_color),
                    cfg.line_thickness
                );
            }
        }

        prev_point = p;
        first_point = false;
    }

    //坐标轴
    draw_list->AddLine(
        ToScreen(x_min, 0), ToScreen(x_max, 0),
        ColorConvertFloat4ToU32(cfg.axis_color), cfg.axis_thickness
    );
    draw_list->AddLine(
        ToScreen(0, -1), ToScreen(0, 1),
        ColorConvertFloat4ToU32(cfg.axis_color), cfg.axis_thickness
    );

    //交互
    if (is_hovered) {
        if (!(flags&PImGuiFunctinPlotFlags_NoZooom)) {

            const float wheel = ImGui::GetIO().MouseWheel;
            if (wheel != 0.0f) {
                const float zoom_factor = 1.1f;
                scale *= (wheel > 0) ? zoom_factor : 1.0f / zoom_factor;
                scale = std::clamp(scale, 0.5f, 10.0f); //限制缩放范围
            }
        }


        is_clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    }


    draw_list->PopClipRect();

    return is_clicked;
}



