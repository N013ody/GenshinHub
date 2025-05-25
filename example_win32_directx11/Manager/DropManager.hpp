#pragma once
#include <wtypes.h>
#include <unordered_map>
#include <mutex>
#include <memory>
#include"DropFile.hpp"
#include <shared_mutex>
#include "CallBackManager.h"



struct BindData {
    HWND hwnd;
    ImGuiID id;

    bool operator==(const BindData& other) const {
        return hwnd == other.hwnd && id == other.id;
    }
};

namespace std {
    template<>
    struct hash<BindData> {
        size_t operator()(const BindData& key) const {
            return (hash<HWND>{}(key.hwnd) << 16) | hash<ImGuiID>{}(key.id);
        }
    };
}


inline std::unordered_map<BindData, std::unique_ptr<DropFile>> g_WindowDropTargets;
inline std::unordered_map<HWND, DropFile*> g_HwndIndex;  
inline std::unordered_map<ImGuiID, DropFile*> g_IDIndex;  


inline std::shared_mutex g_ContainerMutex;

struct OleManager {
    OleManager() { OleInitialize(nullptr); }
    ~OleManager() { OleUninitialize(); }
};

inline void RegisterWindowDropTarget(HWND hwnd, ImGuiID id) {
    std::unique_lock lock(g_ContainerMutex);
    BindData key{ hwnd, id };

    if (!g_WindowDropTargets.count(key)) {
        auto dropTarget = std::make_unique<DropFile>();
        if (SUCCEEDED(RegisterDragDrop(hwnd, dropTarget.get()))) {
            dropTarget->AttachToWindow(hwnd);

    
            g_WindowDropTargets.emplace(key, std::move(dropTarget));
            g_HwndIndex[hwnd] = g_WindowDropTargets[key].get();
            g_IDIndex[id] = g_WindowDropTargets[key].get();
        }
    }
}


inline void UnregisterWindowDropTarget(HWND hwnd, ImGuiID id) {
    std::unique_lock lock(g_ContainerMutex);
    BindData key{ hwnd, id };

    if (auto it = g_WindowDropTargets.find(key); it != g_WindowDropTargets.end()) {
        RevokeDragDrop(hwnd);
        g_HwndIndex.erase(hwnd);
        g_IDIndex.erase(id);
        g_WindowDropTargets.erase(it);
    }
}


inline DropFile* GetDropFileByHwnd(HWND hwnd) {
    std::shared_lock lock(g_ContainerMutex);
    auto it = g_HwndIndex.find(hwnd);
    return (it != g_HwndIndex.end()) ? it->second : nullptr;
}

inline DropFile* GetDropFileByID(ImGuiID id) {
    std::shared_lock lock(g_ContainerMutex);
    auto it = g_IDIndex.find(id);
    return (it != g_IDIndex.end()) ? it->second : nullptr;
}

//回调集成
inline void DropFile_CreateWindowCallBack(ImGuiViewport* vp) {
    if (auto hwnd = (HWND)vp->PlatformHandleRaw;
        hwnd && (ImGui::FindWindowByID(vp->ID)->Flags & ImGuiWindowFlags_DropFiles)) {
        RegisterWindowDropTarget(hwnd, vp->ID);
    }
}

inline void DropFile_DestroyWindowCallBack(ImGuiViewport* vp) {
    UnregisterWindowDropTarget((HWND)vp->PlatformHandleRaw, vp->ID);
}

//初始化
inline HRESULT InitImGuiWithDragDrop() {
    static OleManager ole; 

    CallBackManager& callback = CallBackManager::GetInstance();
    callback.RegesiterCallBack(DropFile_CreateWindowCallBack, CALLBACK_CREATE_WINDOW, POST);
    callback.RegesiterCallBack(DropFile_DestroyWindowCallBack, CALLBACK_DESTROY_WINDOW, POST);

    return S_OK;
}
