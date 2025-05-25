#include "pimgui.h"
#include <wtypes.h>

#include <DropManager.hpp>

//获得当前窗口拖放进的文件路径
const std::vector<std::string>& PImGui::GetCurrentWindowFiles() {
    IM_ASSERT(ImGui::GetCurrentWindow()->Flags & ImGuiWindowFlags_DropFiles
        && "Call this only for windows with ImGuiWindowFlags_DropFiles");

    if (DropFile* dropFile = GetDropFileByID(ImGui::GetCurrentWindow()->ID)) {
        return dropFile->files;
    }

    static const std::vector<std::string> empty;
    return empty;
}


