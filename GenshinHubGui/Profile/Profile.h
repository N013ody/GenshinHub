
#pragma once
#include <wtypes.h>
#include <DX11Util.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <SVGCacheManager.hpp>
#include <LottieAnimator.h>


namespace Global {

    /**
     * @brief SVG缓存管理器实例
     */
    extern SVGCacheManager svgCacheManager;

    /**
     * @brief Lottie动画器实例
     */
    extern LottieAnimator lottieAnimator;

    /**
     * @brief 屏幕宽度
     */
    extern int screenWidth;
    /**
     * @brief 屏幕高度
     */
    extern int screenHeight;
}


namespace Application {
    /**
     * @brief 应用是否已初始化
     */
    extern bool isInitialized;
    /**
     * @brief 应用是否需要退出
     */
    extern bool Exit;
}


namespace Procs {

    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

}

namespace Window {

    extern HWND BackendWindow;

    extern DirectXData WindowDatas;

    extern WNDCLASSEXW BackendWC;

}
