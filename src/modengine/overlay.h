#pragma once

#include <imgui.h>
#include <d3d11.h>
#include <mutex>
#include <nano_signal_slot.hpp>
#include <imterm/misc.hpp>
#include <imterm/terminal.hpp>
#include <imterm/terminal_helpers.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace modengine {

struct custom_command_struct {
    bool should_close = false;
};

class OverlayTerminalHelpers : public ImTerm::basic_spdlog_terminal_helper<OverlayTerminalHelpers, void, std::mutex> {
public:
    friend class Overlay;

    static std::vector<std::string> no_completion(argument_type&) { return {}; }
    static std::vector<std::string> configure_term_autocomplete(argument_type&)
    {
        return {};
    }

    OverlayTerminalHelpers() { }
};

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

    void add_ui_callback(std::function<void()> callback)
    {
        m_on_render.connect(callback);
    }

    ImTerm::terminal<OverlayTerminalHelpers>& get_terminal()
    {
        return m_terminal;
    }

private:
    std::atomic<bool> m_visible;
    Nano::Signal<void()> m_on_render;
    std::once_flag m_initialized;
    ID3D11RenderTargetView* m_render_target_view;
    ID3D11DeviceContext* m_device_context;
    ImTerm::terminal<OverlayTerminalHelpers> m_terminal;
};

}
