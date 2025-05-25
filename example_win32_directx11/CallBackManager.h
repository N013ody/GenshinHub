#pragma once
#include <vector>
#include <mutex>
#include <algorithm>
#include <imgui.h>
#include <imgui_internal.h>

#define CHECK_VIEWPORT(vp, ...) \
    do { \
        if ((vp) == nullptr) { \
            IM_ASSERT(false && "Invalid ImGuiViewport " ##__VA_ARGS__); \
            return; \
        } \
    } while(0)

#define CHECK_VALID_VIEWPORT(vp, ...) \
    do { \
        if ((vp) == nullptr||(vp->PlatformUserData == nullptr)) { \
            IM_ASSERT(false && "Invalid ImGuiViewport or PlatformUserData! " ##__VA_ARGS__); \
            return; \
        } \
    } while(0)

using PlatformCallBack = void (*)(ImGuiViewport* vp);

enum CallBackType {
    CALLBACK_CREATE_WINDOW,
    CALLBACK_DESTROY_WINDOW,
};

enum Pos {
    PRE,
    POST,
};

struct CallBackInfo {
    PlatformCallBack callbackPtr;
    CallBackType type;
    Pos pos;
};

class CallBackManager {
private:
   
    static CallBackManager* instance;
    static std::once_flag initFlag;
    static std::mutex callbacksMutex;

    PlatformCallBack OriginPlatform_CreateWindow;
    PlatformCallBack OriginPlatform_DestroyWindow;
    ImGuiPlatformIO& io;
    std::vector<CallBackInfo> CallBacks;

    //…æµÙ ±ÿ–Î∞≤≈≈
    CallBackManager(const CallBackManager&) = delete;
    CallBackManager& operator=(const CallBackManager&) = delete;

    CallBackManager(ImGuiPlatformIO& platform_io);


    static void Hooked_CreateWindow(ImGuiViewport* vp);
    static void Hooked_DestroyWindow(ImGuiViewport* vp);


public:

    static CallBackManager& GetInstance(ImGuiPlatformIO* io = nullptr) {
        std::call_once(initFlag, [&] {
            IM_ASSERT(io != nullptr);   
            instance = new CallBackManager(*io);
            });
        return *instance;
    };


    static void DestroyInstance() {
        delete instance;
        instance = nullptr;
    }

    ~CallBackManager();


    void SetUpCallBack();
    bool RegesiterCallBack(PlatformCallBack callback, CallBackType type, Pos pos);
    bool UnRegesiterCallBack(PlatformCallBack callback);


};
