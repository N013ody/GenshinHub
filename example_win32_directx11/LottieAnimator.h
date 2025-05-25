#pragma once
#include <rlottie.h>
#include <d3d11.h>
#include <imgui.h>
#include <vector>
#include <memory>

class LottieAnimator {
public:
    LottieAnimator();
    ~LottieAnimator();


    bool Init(const char* jsonPath, ID3D11Device* device,const int width,const int height,const bool upSampling=true);
    bool InitFromData(const char* jsonData, ID3D11Device* device, const int width, const int height, const bool upSampline=true);

    ImVec2 GetSize();

    ID3D11Texture2D* GetTexture();


    void Play() { _isPlaying = true; }         
    void Pause() { _isPlaying = false; }        
    void Stop();                               
    void Seek(float progress);                 
    void SetSpeed(float speed) { _speed = speed; } 
    void Update(float deltaTime);

    ImTextureID GetTextureID() const { return _srv; }
    std::unique_ptr<rlottie::Animation>     GetAnimation();


    void Release();

private:
    void _UpdateTexture();  //内部纹理更新
    bool _InitializeD3DResources( ID3D11Device* device); //初始化D3D资源

    std::unique_ptr<rlottie::Animation> _animation;
    ID3D11Texture2D* _texture = nullptr;
    ID3D11ShaderResourceView* _srv = nullptr;
    ID3D11DeviceContext* _context = nullptr;
    float                _progress = 0.0f;
    float                _speed = 1.0f;
    bool                 _isPlaying = true;
    bool                 _upSampling = false;
    int                  _width, _height;
    int                  _renderWidth, _renderHeight;
    float     supersampleScale = 2.0f; //2倍采样


};
