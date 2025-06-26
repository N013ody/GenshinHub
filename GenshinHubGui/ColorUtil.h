#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <string>

ImVec4 HexToImVec4(const std::string& hex, float alpha = 1.0f);
ImColor HexToImColor(const std::string& hex, float alpha = 1.0f);
