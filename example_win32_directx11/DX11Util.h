#pragma once
#include <d3d11.h>
#include <D3DX11tex.h>
#include "imgui.h"




struct DirectXData {

    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    IDXGISwapChain* g_pSwapChain = nullptr;
    bool                     g_SwapChainOccluded = false;
    UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
};

struct SVGTexture {
    ID3D11ShaderResourceView* srv;
    ImVec2 size;
};


SVGTexture CreateSVGTexture(ID3D11Device* device, const char* svgPath, float scale=1.0f);

bool CreateSRV(ID3D11Device* device, LPCVOID pData, size_t size, ID3D11ShaderResourceView* ptr);

void Cleanup(IDXGISwapChain* swapChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView);

bool CreateDeviceD3D(HWND hWnd);

void CleanupDeviceD3D(ID3D11Device* g_pd3dDevice, IDXGISwapChain* g_pSwapChain, ID3D11DeviceContext* g_pd3dDeviceContext);

void CleanupRenderTarget(ID3D11RenderTargetView* g_renderView);

void CreateRenderTarget(ID3D11Device* g_pd3dDevice, IDXGISwapChain* g_pSwapChain, ID3D11RenderTargetView* g_renderView);

bool InitDirectX(HWND hwnd, WNDCLASSEXW wc);

void Shutdown(HWND BackendWindow, DirectXData WindowDats);
