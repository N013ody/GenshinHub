#pragma once
#include <vector>
#include <imgui_internal.h>
#include <string>
#include <functional>



struct FunctionPlotConfig {
    ImVec4      line_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    ImVec4      grid_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    ImVec4      axis_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    float       axis_thickness = 2.0f;
    float       line_thickness = 2.0f;
    float       grid_spacing = 1.0f;
    float       grid_thickness = 1.0f;
    const char* x_label = "X";                               
    const char* y_label = "Y";                                
};

enum PImGuiFunctinPlotFlags:int {
    PImGuiFunctinPlotFlags_None = 0,      // 0b00
    PImGuiFunctinPlotFlags_NoZooom = 1 << 0, // 0b01
    PImGuiFunctinPlotFlags_NoGrid = 1 << 1,  // 0b10
};

enum PImGuiTextPopupFlags:int{
	PImGuiTextPopupFlags_None = 0,      // 0b00
	PImGuiTextPopupFlags_NoPaste = 1 << 0, // 0b01
	PImGuiTextPopupFlags_NoCopy = 1 << 1  // 0b10
};



namespace PImGui {

    const std::vector<std::string>& GetCurrentWindowFiles();

    bool InputTextPopup(const char* str_id, char* buffer, PImGuiTextPopupFlags popup_flags= PImGuiTextPopupFlags_None);


    bool FunctionPlot(const char* label, float(*func)(float x), float x_min, float x_max, const ImVec2& size,
        const FunctionPlotConfig* config, PImGuiFunctinPlotFlags flags= PImGuiFunctinPlotFlags_None);



}


