#pragma once

// local
#include "screen_renderer.hpp"

class SDL_Window;
class SDL_Renderer;

class ImGuiHandler
{
private:

    ScreenRenderer* renderer;

    ImTextureID get_imgui_texture(TextureIDHolder& texture);
    ImVec2 get_imgui_size(TextureIDHolder& texture, std::optional<glm::u64vec2> const draw_size);


public:

    ImGuiHandler(SDL_Window* window, SDL_Renderer* sdl_renderer, ScreenRenderer* screen_renderer);
    ~ImGuiHandler();

    void image(TextureIDHolder& texture,
               std::optional<glm::u64vec2> const draw_size = std::nullopt);

    bool image_button(std::string const& name, TextureIDHolder& texture,
                      std::optional<glm::u64vec2> const draw_size);
    void start_frame();
    void end_frame();
};