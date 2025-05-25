#pragma once
#include <imgui.h>

enum PlayDirection {
    PLAY_RIGHT,
    PLAY_LEFT,
    PLAY_UP,
    PLAY_DOWN
};

struct AnimParams {
    float   currentTime = 0.0f;     // 累计时间
    int     currentFrame = 0;         // 当前帧序号
    bool    isCompleted = false;     // 是否完成播放
    bool    isLooping = true;        // 是否循环播放
    float   actualFPS = 0.0f;       // 实际动画帧率
    float   fpsTimer = 0.0f;        // 帧率统计计时器
    int     fpsFrames = 0;           // 帧数累计
};

bool PlayImage(ImTextureID texture, ImVec2 size, int totalFrames, PlayDirection direction, float frameDuration, AnimParams& params,
    bool resetOnStart = false,bool debugText=false);

