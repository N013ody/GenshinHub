#include <d3d11.h>
#include <imgui.h>
#include <corecrt_math.h>
#include "ImageUtil.h"

bool PlayImage(ImTextureID texture, ImVec2 size, int totalFrames, PlayDirection direction,
    float frameDuration, AnimParams& params, bool resetOnStart )
{
    // ������Ч�Լ��
    if (!texture || totalFrames <= 0 || frameDuration <= 0.0f)
        return false;

    // ��̬��С֡������ƣ���ֹ����ˢ���ʣ�
    static float minFrameDuration = 1.0f / ImGui::GetIO().Framerate;
    frameDuration = (frameDuration < minFrameDuration) ? minFrameDuration : frameDuration;

    // ���ö���״̬
    if (resetOnStart && params.isCompleted) {
        params.currentTime = 0.0f;
        params.currentFrame = 0;
        params.isCompleted = false;
        params.fpsTimer = 0.0f;
        params.fpsFrames = 0;
    }

    // ʱ��������������
    float deltaTime = ImGui::GetIO().DeltaTime;
    if (!params.isCompleted) {
        int previousFrame = params.currentFrame;
        params.currentTime += deltaTime;
        params.currentFrame = static_cast<int>(params.currentTime / frameDuration);

        // ����֡��ͳ��
        if (params.currentFrame != previousFrame) {
            params.fpsFrames++;
            params.fpsTimer += deltaTime;

            if (params.fpsTimer >= 1.0f) {
                params.actualFPS = params.fpsFrames / params.fpsTimer;
                params.fpsTimer = 0.0f;
                params.fpsFrames = 0;
            }
        }

        // ѭ��/��ֹ�߼�
        if (params.currentFrame >= totalFrames) {
            if (params.isLooping) {
                params.currentFrame %= totalFrames;
                params.currentTime = fmod(params.currentTime, frameDuration * totalFrames);
            }
            else {
                params.currentFrame = totalFrames - 1;
                params.isCompleted = true;
            }
        }
    }

    // UV�������
    ImVec2 uv0, uv1;
    const float frameUnit = 1.0f / totalFrames;
    switch (direction) {
    case PLAY_RIGHT:
        uv0 = ImVec2(params.currentFrame * frameUnit, 0.0f);
        uv1 = ImVec2((params.currentFrame + 1) * frameUnit, 1.0f);
        break;
    case PLAY_LEFT:
        uv0 = ImVec2(1.0f - (params.currentFrame + 1) * frameUnit, 0.0f);
        uv1 = ImVec2(1.0f - params.currentFrame * frameUnit, 1.0f);
        break;
    case PLAY_DOWN:
        uv0 = ImVec2(0.0f, params.currentFrame * frameUnit);
        uv1 = ImVec2(1.0f, (params.currentFrame + 1) * frameUnit);
        break;
    case PLAY_UP:
        uv0 = ImVec2(0.0f, 1.0f - (params.currentFrame + 1) * frameUnit);
        uv1 = ImVec2(1.0f, 1.0f - params.currentFrame * frameUnit);
        break;
    }

    // ��Ⱦͼ�񼰵�����Ϣ
    ImGui::Image(texture, size, uv0, uv1);
    ImGui::Text("Frame: %d/%d | AnimFPS: %.1f | RenderFPS: %.1f",
        params.currentFrame + 1, totalFrames,
        params.actualFPS, ImGui::GetIO().Framerate);

    return true;
}
