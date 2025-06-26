#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_utils.h"
#include <Utils/MathUtil.h>

int rotation_start_index;

void ImRotateStart()
{
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}


ImVec2 ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}


void rect_glow(ImDrawList* draw, ImVec2 start, ImVec2 end, ImColor col, float rounding, float intensity) {
    while (true) {
        if (col.Value.w < 0.0019f)
            break;

        draw->AddRectFilled(start, end, col, rounding);

        col.Value.w -= col.Value.w / intensity;
        start = ImVec2(start.x - 1, start.y - 1);
        end = ImVec2(end.x + 1, end.y + 1);
    }
}

inline void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
{
    float s = sin(rad), c = cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}


void ImRotateTo90Degrees(float& notif_rotate, bool screen_coord = false)
{

    float target_deg = 90.0f;
    if (screen_coord) target_deg = 360.0f - target_deg;

    // Æ½½Ç
    target_deg = normalizeFlatAngle(target_deg);

    // ×ª»¡¶È
    float target_rad = target_deg * (IM_PI / 180.0f);

    notif_rotate = target_rad;


    ImRotateEnd(notif_rotate);
}


static ImVec2 InputTextCalcTextSizeW(ImGuiContext* ctx, const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining, ImVec2* out_offset, bool stop_on_new_line)
{
    ImGuiContext& g = *ctx;
    ImFont* font = g.Font;
    const float line_height = g.FontSize;
    const float scale = line_height / font->FontSize;

    ImVec2 text_size = ImVec2(0, 0);
    float line_width = 0.0f;

    const ImWchar* s = text_begin;
    while (s < text_end)
    {
        unsigned int c = (unsigned int)(*s++);
        if (c == '\n')
        {
            text_size.x = ImMax(text_size.x, line_width);
            text_size.y += line_height;
            line_width = 0.0f;
            if (stop_on_new_line)
                break;
            continue;
        }
        if (c == '\r')
            continue;

        const float char_width = font->GetCharAdvance((ImWchar)c) * scale;
        line_width += char_width;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;

    if (out_offset)
        *out_offset = ImVec2(line_width, text_size.y + line_height);  // offset allow for the possibility of sitting after a trailing \n

    if (line_width > 0 || text_size.y == 0.0f)                        // whereas size.y will ignore the trailing \n
        text_size.y += line_height;

    if (remaining)
        *remaining = s;

    return text_size;
}


