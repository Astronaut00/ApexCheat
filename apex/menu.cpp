#include "cheat.h"
#include "render.hpp"
#include <d3d11.h>
#include <thread>
#include <mutex>
#include "textmenu.h"

unsigned char prevKeys[256];
unsigned char keys[256];

ID3D11ShaderResourceView* logo_texture = NULL;
void Cheat::Menu()
{
    if (textMenu->enabled) {
        bool* p_open = NULL;
        ImGui::SetNextWindowSize(ImVec2(140, 180));
        ImGui::SetNextWindowPos(ImVec2(5, 5));
        ImGui::Begin("begin", p_open, ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration);
        auto dimensions = ImVec2(28, 28);
        ImGui::SetCursorPos(ImVec2(8, 8));
        ImGui::Image((void*)logo_texture, dimensions);
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(40, 15));
        ImGui::Text("Exomoon");

        textMenu->AddMenuItem("Glow", &settings.playerGlow);
        textMenu->AddMenuItem("Box", &settings.playerBox);
        textMenu->AddMenuItem("Health", &settings.playerHealth);
       // textMenu->AddMenuItem("Radar", &settings.playerHealth);
        textMenu->AddMenuItem("Items", &settings.itemName);
        textMenu->AddMenuItem("Aimbot", (int*)&settings.aimbotMode, std::vector<std::string>{"OFF", "LEGIT", "ASSIST","RAGE"});
        textMenu->AddMenuItem("Aimbot FOV", &settings.fov);
        textMenu->AddMenuItem("Aimbot Smooth", &settings.smoothing);
        textMenu->RenderMenuItems();
        ImGui::End();
    }
    textMenu->HandleInput();
}


LRESULT __stdcall Cheat::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{ 
    switch (msg)
    {
    case WM_CREATE:
        return 0l;
    case WM_KEYDOWN:
            std::cout << wParam << std::endl;
            return 0L;
        break;
    case WM_SIZE:
        if (pCheat->g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            pCheat->CleanupRenderTarget();
            pCheat->g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            pCheat->CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
