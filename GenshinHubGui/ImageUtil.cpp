#include <d3d11.h>
#include <imgui.h>
#include <corecrt_math.h>
#include "ImageUtil.h"

bool PlayImage(ImTextureID texture, ImVec2 size, int totalFrames, PlayDirection direction,
    float frameDuration, AnimParams& params, bool resetOnStart,bool debugText )
{

    if (!texture || totalFrames <= 0 || frameDuration <= 0.0f)
        return false;

    //动态最小帧间隔限制
    static float minFrameDuration = 1.0f / ImGui::GetIO().Framerate;
    frameDuration = (frameDuration < minFrameDuration) ? minFrameDuration : frameDuration;

    //重置动画状态
    if (resetOnStart && params.isCompleted) {
        params.currentTime = 0.0f;
        params.currentFrame = 0;
        params.isCompleted = false;
        params.fpsTimer = 0.0f;
        params.fpsFrames = 0;
    }

   
    float deltaTime = ImGui::GetIO().DeltaTime;
    if (!params.isCompleted) {
        int previousFrame = params.currentFrame;
        params.currentTime += deltaTime;
        params.currentFrame = static_cast<int>(params.currentTime / frameDuration);

      
        if (params.currentFrame != previousFrame) {
            params.fpsFrames++;
            params.fpsTimer += deltaTime;

            if (params.fpsTimer >= 1.0f) {
                params.actualFPS = params.fpsFrames / params.fpsTimer;
                params.fpsTimer = 0.0f;
                params.fpsFrames = 0;
            }
        }

      
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


    ImGui::Image(texture, size, uv0, uv1);
    if(debugText)
    ImGui::Text("Frame: %d/%d | AnimFPS: %.1f | RenderFPS: %.1f",
        params.currentFrame + 1, totalFrames,
        params.actualFPS, ImGui::GetIO().Framerate);

    return true;
}

ID3D11ShaderResourceView* LoadTextureFromMemory(
    ID3D11Device* device,
    const uint8_t* data,
    size_t size
) {
    ID3D11ShaderResourceView* srv = nullptr;
    HRESULT hr = DirectX::CreateWICTextureFromMemoryEx(
        device,
        data, size,
        0, 
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE,
        0, 0, 
        DirectX::DX11::WIC_LOADER_FLAGS::WIC_LOADER_IGNORE_SRGB,//修复纹理颜色
        nullptr, &srv
    );
    if (FAILED(hr)) {
        return nullptr;
    }
    return srv;
}
