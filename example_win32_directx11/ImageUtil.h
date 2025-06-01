#pragma once
#include <imgui.h>

enum PlayDirection {
    PLAY_RIGHT,
    PLAY_LEFT,
    PLAY_UP,
    PLAY_DOWN
};

struct AnimParams {
    float   currentTime = 0.0f;     // �ۼ�ʱ��
    int     currentFrame = 0;         // ��ǰ֡���
    bool    isCompleted = false;     // �Ƿ���ɲ���
    bool    isLooping = true;        // �Ƿ�ѭ������
    float   actualFPS = 0.0f;       // ʵ�ʶ���֡��
    float   fpsTimer = 0.0f;        // ֡��ͳ�Ƽ�ʱ��
    int     fpsFrames = 0;           // ֡���ۼ�
};

bool PlayImage(ImTextureID texture, ImVec2 size, int totalFrames, PlayDirection direction, float frameDuration, AnimParams& params,
    bool resetOnStart = false,bool debugText=false);

