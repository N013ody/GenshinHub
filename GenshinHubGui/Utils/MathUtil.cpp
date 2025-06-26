#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <cmath>

float normalizeFlatAngle(float degrees) {
    degrees = fmodf(degrees, 360.0f); 
    if (degrees < 0) degrees += 360.0f; 
    // Æ½½Ç×ª»»
    return (degrees > 180.0f) ? 360.0f - degrees : degrees;
}
ImVec2 center_text(ImVec2 min, ImVec2 max, const char* text)
{
    return min + (max - min) / 2 - ImGui::CalcTextSize(text) / 2;
}
