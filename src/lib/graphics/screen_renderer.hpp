#pragma once

// builtin
#include <glm/ext/vector_uint2_sized.hpp>
#include <string_view>
#include <unordered_map>

// local
#include "../core/camera.hpp"
#include "../core/constants.hpp"
#include "../utils/assert.hpp"
#include "../utils/print_utils.hpp"
#include "texture.hpp"

// external
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <fmt/format.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <imgui_impl_sdlrenderer2.h>

// TODO: support HIDPI displays


class ScreenRenderer
{
    friend class GameContext;

protected:

    SDL_Renderer* renderer = nullptr;

private:

    size_t next_texture_id = 0;
    std::unordered_map<size_t, SDLRendererTexture> textures;
    Camera const* const camera;

public:

    explicit ScreenRenderer(SDL_Window* window, Camera const* const camera);

    ~ScreenRenderer();


    TextureID load_texture(std::string_view const path);
    SDLRendererTexture& get_texture(TextureIDHolder& texture_holder);

    void clear(glm::u8vec3 const clear_color = {0, 0, 0});

    void draw_texture(TextureIDHolder const& texture, glm::vec2 const world_position,
                      std::optional<glm::u64vec2> custom_draw_size = std::nullopt,
                      std::optional<glm::u8vec3> modulate          = std::nullopt);

    void draw_rectangle(glm::vec2 const world_position, glm::u64vec2 const size,
                        glm::u8vec3 const color);

    void draw_line(glm::u64vec2 const start, glm::u64vec2 const end, glm::u8vec3 const color);

    void start_frame();

    void present();

    SDLRendererTexture& get_texture(TextureIDHolder const& texture);

private:


    glm::vec2 world_to_screen_position(glm::vec2 const world_position) const;

    bool is_visible(glm::vec2 const pos, glm::vec2 const size) const;

    SDLRendererTexture load_sdl_texture(std::string_view const path);
};
