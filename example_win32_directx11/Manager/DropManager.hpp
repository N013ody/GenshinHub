#pragma once
#include <unordered_map>
#include <DropFile.hpp>
#include <imgui_internal.h>
#include <CallBackManager.h>


inline std::unordered_map<HWND, DropFile*> g_WindowDropTargets;//确保全局唯一性

inline  void    (*OriginPlatform_CreateWindow)(ImGuiViewport* vp);
inline  void    (*OriginPlatform_DestoryWindow)(ImGuiViewport* vp);

inline void RegisterWindowDropTarget(HWND hwnd) {
    if (g_WindowDropTargets.find(hwnd) == g_WindowDropTargets.end()) {
        DropFile* dropTarget = new DropFile();
        if (SUCCEEDED(RegisterDragDrop(hwnd, dropTarget))) {
            dropTarget->AttachToWindow(hwnd);
            g_WindowDropTargets[hwnd] = dropTarget;
        }
    }
}

inline void UnregisterWindowDropTarget(HWND hwnd) {
    if (auto it = g_WindowDropTargets.find(hwnd); it != g_WindowDropTargets.end()) {
        RevokeDragDrop(hwnd);
        it->second->Release(); 
        g_WindowDropTargets.erase(it);
    }
}


inline DropFile* GetDropFileByHwnd(HWND hwnd) {
    if (hwnd == NULL)return nullptr;
    auto it = g_WindowDropTargets.find(hwnd);
    if (it != g_WindowDropTargets.end()) {
        return it->second;
    }
    return nullptr; 
}



inline void DropFile_CreateWindowCallBack(ImGuiViewport* vp) {

    ImGuiPlatformIO& io = ImGui::GetPlatformIO();


    ImGuiID viewport_id = vp->ID; 
    ImGuiWindow* window = ImGui::FindWindowByID(viewport_id);

    HWND hwnd = (HWND)vp->PlatformHandleRaw;
    if (hwnd && (window->Flags & ImGuiWindowFlags_DropFiles)) {
         RegisterWindowDropTarget(hwnd);

    }
}


inline void DropFile_DestroyWindowCallBack(ImGuiViewport* vp) {
    OleUninitialize();  
}


inline HRESULT InitImGuiWithDragDrop() {
    HRESULT result = OleInitialize(NULL);
    CallBackManager& callback=CallBackManager::GetInstance();  
    callback.RegesiterCallBack(DropFile_CreateWindowCallBack, CALLBACK_CREATE_WINDOW, POST);
    callback.RegesiterCallBack(DropFile_DestroyWindowCallBack, CALLBACK_DESTROY_WINDOW, POST);
    return result;
}
