// header
#include "./imgui_handler.hpp"

// builtin
#include <iostream>

// external
#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

// local
#include "../utils/assert.hpp"



ImGuiHandler::ImGuiHandler(SDL_Window* window, SDL_Renderer* sdl_renderer,
                           ScreenRenderer* screen_renderer): renderer{screen_renderer}
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io{ImGui::GetIO()};

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;


    bool status = ImGui_ImplSDL2_InitForSDLRenderer(window, sdl_renderer);
    ib_runtime_assert(status, "Failed to init ImGui for SDLRenderer2");
    status = ImGui_ImplSDLRenderer2_Init(sdl_renderer);
    ib_runtime_assert(status, "Failed to init ImGui SDLRenderer2");
}

ImGuiHandler::~ImGuiHandler()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
ImTextureID ImGuiHandler::get_imgui_texture(TextureIDHolder& texture)
{
    SDLRendererTexture const& sdl_texture = this->renderer->get_texture(texture);
    ImTextureID imgui_texture             = (ImTextureID)(intptr_t)sdl_texture.texture;

    return imgui_texture;
}
ImVec2 ImGuiHandler::get_imgui_size(TextureIDHolder& texture,
                                    std::optional<glm::u64vec2> const draw_size)
{
    SDLRendererTexture const& sdl_texture = this->renderer->get_texture(texture);

    ImVec2 size;
    if (!draw_size.has_value())
    {
        auto texture_size = sdl_texture.get_size();
        size.x            = texture_size.x;
        size.y            = texture_size.y;
    }
    else
    {
        size.x = draw_size.value().x;
        size.y = draw_size.value().y;
    }

    return size;
}


void ImGuiHandler::image(TextureIDHolder& texture, std::optional<glm::u64vec2> const draw_size)
{
    ImTextureID imgui_texture = this->get_imgui_texture(texture);
    ImVec2 size               = this->get_imgui_size(texture, draw_size);

    ImGui::Image(imgui_texture, size);
}

bool ImGuiHandler::image_button(std::string const& name, TextureIDHolder& texture,
                                std::optional<glm::u64vec2> const draw_size)
{
    ImTextureID imgui_texture = this->get_imgui_texture(texture);
    ImVec2 size               = this->get_imgui_size(texture, draw_size);

    ImGui::BeginTable(("ImageButtonTable##" + name).c_str(), 1, ImGuiTableFlags_SizingFixedFit);
    ImGui::TableNextColumn();
    bool const pressed = ImGui::ImageButton(("ImageButton##" + name).c_str(), imgui_texture, size);
    float const collum_size     = ImGui::GetColumnWidth(-1);
    float const text_size       = ImGui::CalcTextSize(name.c_str()).x;
    float const text_center_pos = (collum_size - text_size) / 2;
    ImGui::SetCursorPosX(ImGui::GetCursorPos().x + text_center_pos);
    ImGui::TextUnformatted(name.c_str());
    ImGui::EndTable();

    return pressed;
}

void ImGuiHandler::start_frame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void ImGuiHandler::end_frame()
{
    ImGui::Render();
}
