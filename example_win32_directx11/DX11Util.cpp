#define NANOSVG_IMPLEMENTATION    // 解析器实现
#define NANOSVGRAST_IMPLEMENTATION // 光栅化器实现

#include "DX11Util.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "Profile.h"
#include <wrl/client.h> 
#include <filesystem>
#include "nanosvg.h"
#include "nanosvgrast.h"

using Microsoft::WRL::ComPtr;  
using namespace Window;

SVGTexture CreateSVGTexture(ID3D11Device* device, const char* svgPath, float scale) {
    //文件存在
    if (!std::filesystem::exists(svgPath)) {
        return {};
    }

    //SVG解析
    NSVGimage* svgImage = nsvgParseFromFile(svgPath, "px", 96.0f);
    if (!svgImage || svgImage->width <= 0 || svgImage->height <= 0) {
        return {};
    }

    //计算缩放尺寸
    int width = static_cast<int>(svgImage->width * scale);
    int height = static_cast<int>(svgImage->height * scale);
    if (width == 0 || height == 0) {
        nsvgDelete(svgImage);
        return {};
    }

    //光栅化到内存
    std::vector<unsigned char> pixels(width * height * 4);
    NSVGrasterizer* rasterizer = nsvgCreateRasterizer();
    nsvgRasterize(rasterizer, svgImage, 0, 0, scale,
        pixels.data(), width, height, width * 4);
    nsvgDeleteRasterizer(rasterizer);

    //ARGB转RGBA
    for (int i = 0; i < width * height; ++i) {
        uint32_t argb = *reinterpret_cast<uint32_t*>(&pixels[i * 4]);
        pixels[i * 4 + 0] = (argb >> 16) & 0xFF; //R
        pixels[i * 4 + 1] = (argb >> 8) & 0xFF;  //G
        pixels[i * 4 + 2] = argb & 0xFF;         //B
        pixels[i * 4 + 3] = (argb >> 24) & 0xFF; //A
    }

    // 6. 创建DX11纹理（内存对齐优化）
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = pixels.data();
    initData.SysMemPitch = (width * 4 ) ; //128字节对齐

    ComPtr<ID3D11Texture2D> texture;
    HRESULT hr = device->CreateTexture2D(&desc, &initData, &texture);
    if (FAILED(hr)) {
        nsvgDelete(svgImage);
        return {};
    }

    SVGTexture result;
    hr = device->CreateShaderResourceView(texture.Get(), nullptr, &result.srv);
    if (FAILED(hr)) {
        return {};
    }

    result.size = ImVec2((float)width, (float)height);
    nsvgDelete(svgImage);
    return result;
}

void Shutdown(HWND BackendWindow, DirectXData WindowDats) {
    Cleanup(WindowDatas.g_pSwapChain, WindowDatas.g_pd3dDevice, WindowDatas.g_pd3dDeviceContext, WindowDatas.g_mainRenderTargetView);
    CleanupDeviceD3D(WindowDatas.g_pd3dDevice, WindowDatas.g_pSwapChain, WindowDatas.g_pd3dDeviceContext);
    ::DestroyWindow(BackendWindow);
    ::UnregisterClassW(BackendWC.lpszClassName, BackendWC.hInstance);
}
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







