#include "overlay.h"
#include "mod_engine.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <spdlog/spdlog.h>

#include <windows.h>
#include <mutex>

using namespace spdlog;

namespace modengine {

static WNDPROC game_window_callback;

static LRESULT __stdcall window_msg_handler(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto& io = ImGui::GetIO();

    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) || io.WantCaptureKeyboard || io.WantCaptureMouse) {
        return true;
    }

    return CallWindowProc(game_window_callback, hWnd, uMsg, wParam, lParam);
}

void Overlay::init(HWND window, ID3D11Device* device, ID3D11DeviceContext* device_context)
{
    game_window_callback = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)window_msg_handler);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(device, device_context);

    m_device_context = device_context;

    spdlog::default_logger()->sinks().push_back(m_terminal.get_terminal_helper());
}

void Overlay::render(IDXGISwapChain* swap_chain)
{
    std::call_once(m_initialized, [&]() {
        DXGI_SWAP_CHAIN_DESC desc;
        swap_chain->GetDesc(&desc);

        ID3D11Device* device;
        swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&device);

        ID3D11DeviceContext* context;
        device->GetImmediateContext(&context);

        ID3D11Texture2D* framebuffer;
        swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&framebuffer);
        device->CreateRenderTargetView(framebuffer, NULL, &m_render_target_view);
        framebuffer->Release();

        init(desc.OutputWindow, device, context);
    });

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();



    if (m_visible) {
        m_on_render.fire();
    }

    ImGui::Render();
    m_device_context->OMSetRenderTargets(1, &m_render_target_view, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

}