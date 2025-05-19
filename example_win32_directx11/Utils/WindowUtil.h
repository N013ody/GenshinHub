#pragma once
#include <wtypes.h>
#include <imgui_internal.h>
#include <string>
HWND GetWindowHWND(ImGuiWindow* window);

ImGuiWindow* GetWindowByHwnd(HWND hwnd);

bool AreAllPlatformWindowsClosed();

int SetWindowStyles(HWND hwnd, DWORD styleVar);

const WNDCLASSEXW GetWindowClass(const std::string& name);


