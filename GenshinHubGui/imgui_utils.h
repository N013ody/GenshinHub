#pragma once

#include <imgui.h>
#include <imgui_internal.h>

void ImRotateStart();
ImVec2 ImRotationCenter();
void rect_glow(ImDrawList* draw, ImVec2 start, ImVec2 end, ImColor col, float rounding, float intensity);
static ImVec2 InputTextCalcTextSizeW(ImGuiContext* ctx, const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining, ImVec2* out_offset, bool stop_on_new_line);
