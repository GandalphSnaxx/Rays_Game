#include <windows.h>

#include "renderer/vk_renderer.cpp"

static bool running = true;

LRESULT CALLBACK platform_window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    LRESULT result = 0;

    switch (message) {
        case WM_CLOSE: {
            running = false;
            DestroyWindow(window);
        } break;

        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;

        default: {
            result = DefWindowProcA(window, message, wparam, lparam);
        } break;
    }

    return result;
}

bool platform_create_window(HWND *window) {
    HINSTANCE instance = GetModuleHandleA(0);

    WNDCLASSA wc = {};
    wc.lpfnWndProc = platform_window_callback;
    wc.hInstance = instance;
    wc.lpszClassName = "vulkan_engine_class";
    wc.hCursor = LoadCursor(0, IDC_ARROW);

    if (!RegisterClassA(&wc)) {
        MessageBoxA(0, "Failed to register window class", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    *window = CreateWindowExA(
        WS_EX_APPWINDOW,
        "vulkan_engine_class",
        "Hello Vulkan",
        WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPED,
        100, 100, 1600, 720, 0, 0, instance, 0
    );

    if (window == NULL) {
        MessageBoxA(0, "Failed to create window", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    ShowWindow(*window, SW_SHOW);

    return true;
}

void platform_update_window(HWND window) {
    MSG message;
    while (PeekMessageA(&message, window, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

int main() {
    VkContext vkcontext = {};

    HWND window = 0;
    if (!platform_create_window(&window)) {
        return -1;
    }

    if (!vk_init(&vkcontext, &window)) {
        return -1;
    }

    while (running) {
        platform_update_window(window);
    }

    return 0;
}