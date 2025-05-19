#pragma once

#include <D3DX11tex.h>
#include <d3d11.h>
#include <Images.h>

static ID3D11ShaderResourceView* playImagePtr = nullptr;
static ID3D11ShaderResourceView* icon_logo = nullptr;

HRESULT GuiMainInit();

void RenderMainWindow();


