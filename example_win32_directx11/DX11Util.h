#pragma once
#include <d3d11.h>
#include <D3DX11tex.h>

struct DirectXData {

    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    IDXGISwapChain* g_pSwapChain = nullptr;
    bool                     g_SwapChainOccluded = false;
    UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
};


class float2 {
public:
    float x;
    float y;


    float2() : x(0.0f), y(0.0f) {}

    
    float2(float _x, float _y) : x(_x), y(_y) {}


    float2 operator+(const float2& other) const {
        return float2(x + other.x, y + other.y);
    }

    // 向量点积
    float dot(const float2& other) const {
        return x * other.x + y * other.y;
    }
};

bool CreateSRV(ID3D11Device* device, LPCVOID pData, size_t size, ID3D11ShaderResourceView* ptr);
void Cleanup(IDXGISwapChain* swapChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView);
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D(ID3D11Device* g_pd3dDevice, IDXGISwapChain* g_pSwapChain, ID3D11DeviceContext* g_pd3dDeviceContext);

void CleanupRenderTarget(ID3D11RenderTargetView* g_renderView);

void CreateRenderTarget(ID3D11Device* g_pd3dDevice, IDXGISwapChain* g_pSwapChain, ID3D11RenderTargetView* g_renderView);

bool InitDirectX(HWND hwnd, WNDCLASSEXW wc);
