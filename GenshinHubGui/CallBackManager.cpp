#include "CallBackManager.h"
#include <imgui.h>

CallBackManager* CallBackManager::instance;  
std::once_flag CallBackManager::initFlag;          
std::mutex CallBackManager::callbacksMutex;


CallBackManager::CallBackManager(ImGuiPlatformIO& platform_io)
    : io(platform_io) {
    if (!platform_io.Platform_CreateWindow) IM_ASSERT("Platform_CreateWindow is null");
    if (!platform_io.Platform_DestroyWindow) IM_ASSERT("Platform_DestroyWindow is null");
    OriginPlatform_CreateWindow = platform_io.Platform_CreateWindow;
    OriginPlatform_DestroyWindow = platform_io.Platform_DestroyWindow;

}


CallBackManager::~CallBackManager() {
    io.Platform_CreateWindow = OriginPlatform_CreateWindow;
    io.Platform_DestroyWindow = OriginPlatform_DestroyWindow;
}


void CallBackManager::Hooked_CreateWindow(ImGuiViewport* vp) {
    CHECK_VIEWPORT(vp,"Hooked_CreateWindow ViewPort is null");
    auto& instance = CallBackManager::GetInstance();
    vp->PlatformUserData = &instance;

    {
        std::lock_guard<std::mutex> lock(instance.callbacksMutex); 
        for (const auto& cb : instance.CallBacks) {
            if (cb.type == CALLBACK_CREATE_WINDOW && cb.pos == PRE) {
                cb.callbackPtr(vp);
            }
        }
    }

    instance.OriginPlatform_CreateWindow(vp);
    {
        std::lock_guard<std::mutex> lock(instance.callbacksMutex);
        for (const auto& cb : instance.CallBacks) {
            if (cb.type == CALLBACK_CREATE_WINDOW && cb.pos == POST) {
                cb.callbackPtr(vp);
            }
        }
    }

}

void CallBackManager::Hooked_DestroyWindow(ImGuiViewport* vp) {
   // CHECK_VALID_VIEWPORT(vp, "Hooked_DestroyWindow ViewPort or PlatformUserData  is null ,Did you call Hooked_CreateWindow?");
    auto& instance = CallBackManager::GetInstance();
    {
        std::lock_guard<std::mutex> lock(instance.callbacksMutex);
        for (const auto& cb : instance.CallBacks) {
            if (cb.type == CALLBACK_DESTROY_WINDOW && cb.pos == PRE) {
                cb.callbackPtr(vp);
            }
        }
    }
    instance.OriginPlatform_DestroyWindow(vp);

    {
        std::lock_guard<std::mutex> lock(instance.callbacksMutex);
        for (const auto& cb : instance.CallBacks) {
            if (cb.type == CALLBACK_DESTROY_WINDOW && cb.pos == POST) {
                cb.callbackPtr(vp);
            }
        }
    }
    vp->PlatformUserData = nullptr;

}

void CallBackManager::SetUpCallBack() {
    io.Platform_CreateWindow = &Hooked_CreateWindow;
    io.Platform_DestroyWindow = &Hooked_DestroyWindow;
}

bool CallBackManager::RegesiterCallBack(PlatformCallBack callback, CallBackType type, Pos pos) {
    std::lock_guard<std::mutex> lock(callbacksMutex);//线程安全这块
    auto it = std::find_if(CallBacks.begin(), CallBacks.end(),
        [&](const CallBackInfo& ci) {
            return ci.callbackPtr == callback && ci.type == type && ci.pos == pos;
        });
    if (it != CallBacks.end()) return false;
    CallBacks.push_back({ callback, type, pos });
    return true;
}

bool CallBackManager::UnRegesiterCallBack(PlatformCallBack callback) {
    std::lock_guard<std::mutex> lock(callbacksMutex); 
    auto it = std::remove_if(CallBacks.begin(), CallBacks.end(),
        [&](const CallBackInfo& ci) {
            return ci.callbackPtr == callback;
        });
    if (it != CallBacks.end()) {
        CallBacks.erase(it, CallBacks.end());
        return true;
    }
    return false;
}
