#include "DX11Util.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "Profile.h"

using namespace Window;

static bool CreateSRV(ID3D11Device* device, LPCVOID pData, size_t size, ID3D11ShaderResourceView* ptr) {
    HRESULT hr = D3DX11CreateShaderResourceViewFromMemory(device, pData, sizeof(pData), nullptr, nullptr, &ptr, 0);


    if (FAILED(hr)) {
        OutputDebugStringW(L"Unsupported image format");
        return false;
    }

    return true;

}

void Cleanup(IDXGISwapChain* swapChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView) {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}


bool CreateDeviceD3D(HWND hWnd)
{

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &WindowDatas.g_pSwapChain, &WindowDatas.g_pd3dDevice, &featureLevel, &WindowDatas.g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) 
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &WindowDatas.g_pSwapChain, &WindowDatas.g_pd3dDevice, &featureLevel, &WindowDatas.g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget(WindowDatas.g_pd3dDevice, WindowDatas.g_pSwapChain, WindowDatas.g_mainRenderTargetView);
    return true;
}

bool InitDirectX(HWND hwnd,WNDCLASSEXW wc)
{

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D(WindowDatas.g_pd3dDevice, WindowDatas.g_pSwapChain, WindowDatas.g_pd3dDeviceContext);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }


    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(WindowDatas.g_pd3dDevice, WindowDatas.g_pd3dDeviceContext);


}

void CleanupDeviceD3D(ID3D11Device* g_pd3dDevice,IDXGISwapChain* g_pSwapChain, ID3D11DeviceContext* g_pd3dDeviceContext)
{
    CleanupRenderTarget( WindowDatas.g_mainRenderTargetView);
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget(ID3D11Device* g_pd3dDevice,IDXGISwapChain* g_pSwapChain, ID3D11RenderTargetView* g_renderView)
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_renderView);
    pBackBuffer->Release();
}

void CleanupRenderTarget(ID3D11RenderTargetView* g_renderView)
{
    if (g_renderView) { g_renderView->Release(); g_renderView = nullptr; }
}







