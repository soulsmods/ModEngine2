#pragma once

#include <imgui.h>
#include <d3d11.h>
#include <mutex>
#include <nano_signal_slot.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace modengine {

class Overlay {
public:
    void init(HWND window, ID3D11Device* device, ID3D11DeviceContext* device_context);
    void render(IDXGISwapChain* swap_chain);

    bool is_visible()
    {
        return m_visible.load();
    }

    void set_visible(bool visible)
    {
        m_visible = visible;
    }
private:
    std::atomic<bool> m_visible;
    std::once_flag m_initialized;
    ID3D11RenderTargetView* m_render_target_view;
    ID3D11DeviceContext* m_device_context;
};

}
