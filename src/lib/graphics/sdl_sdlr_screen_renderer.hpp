#pragma once

// builtin
#include <glm/ext/vector_uint2_sized.hpp>
#include <string_view>
#include <unordered_map>

// local
#include "../utils/assert.hpp"
#include "../utils/print_utils.hpp"
#include "screen_renderer.hpp"

// external
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <fmt/format.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <imgui_impl_sdlrenderer2.h>

// TODO: support HIDPI displays



struct SDLRScreenRendererTexture
{
public:

    SDL_Texture* texture = nullptr;

public: // ctor's, dtor's, and move semantics

    explicit SDLRScreenRendererTexture(SDL_Texture* const texture): texture{texture} {}
    SDLRScreenRendererTexture(SDLRScreenRendererTexture const&)            = delete;
    SDLRScreenRendererTexture& operator=(SDLRScreenRendererTexture const&) = delete;

    SDLRScreenRendererTexture(SDLRScreenRendererTexture&& other) noexcept: texture{other.texture}
    {
        other.texture = nullptr;
    }

    SDLRScreenRendererTexture& operator=(SDLRScreenRendererTexture&& other) noexcept
    {
        if (this != &other)
        {
            this->texture = other.texture;
            other.texture = nullptr;
        }
        return *this;
    }

    ~SDLRScreenRendererTexture()
    {
        if (this->texture == nullptr)
            return;

        SDL_DestroyTexture(this->texture);
    }

public: // public methods

    [[nodiscard]] glm::u64vec2 get_size() const
    {
        int width, height;
        int result = SDL_QueryTexture(this->texture, nullptr, nullptr, &width, &height);
        ib_runtime_assert(result == 0,
                          fmt::format("Failed to query SDL texture size, {}", SDL_GetError()));
        return glm::u64vec2{(uint64_t)width, (uint64_t)height};
    }
};



class SDLRScreenRenderer final : public ScreenRenderer
{
    friend class GameContext;

protected:

    SDL_Renderer* renderer = nullptr;

private:

    size_t next_texture_id = 0;
    std::unordered_map<size_t, SDLRScreenRendererTexture> textures;

public:

    explicit SDLRScreenRenderer(SDL_Window* window, Camera const* const camera):
        ScreenRenderer{camera}
    {
        this->renderer =
            SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        ib_runtime_assert(this->renderer != nullptr, "Failed to create SDL renderer");
    }

    ~SDLRScreenRenderer() final
    {
        // since the dtor of the class members are called after this dtor, we
        // need to manually trigger the destruction of any object which might
        // depend on the renderer to perform a graceful shutdown.
        this->textures.clear();

        SDL_DestroyRenderer(this->renderer);
    }


    TextureID load_texture(std::string_view const path) override
    {
        SDLRScreenRendererTexture texture = this->load_sdl_texture(path);
        this->textures.insert({this->next_texture_id, std::move(texture)});
        return TextureID{this->next_texture_id++};
    }

    void clear(glm::u8vec3 const clear_color) override
    {
        SDL_SetRenderDrawColor(this->renderer, clear_color.r, clear_color.g, clear_color.b, 255);
        SDL_RenderClear(this->renderer);
    }

    void draw_texture(TextureID const texture, glm::vec2 const world_position,
                      std::optional<glm::u64vec2> custom_draw_size = std::nullopt) override
    {
        SDLRScreenRendererTexture const& sdl_texture = this->textures.at(texture.id);
        glm::u64vec2 const draw_size = custom_draw_size.value_or(sdl_texture.get_size());

        glm::vec2 const screen_position = this->world_to_screen_position(world_position);
        if (!this->is_visible(screen_position, draw_size))
            return;
        SDL_Rect const dest_rect = {(int)screen_position.x, (int)screen_position.y,
                                    (int)draw_size.x, (int)draw_size.y};

        SDL_RenderCopy(this->renderer, sdl_texture.texture, nullptr, &dest_rect);
    }
    void draw_texture(TextureIDHolder const& texture_holder, glm::vec2 const world_position,
                      std::optional<glm::u64vec2> custom_draw_size = std::nullopt) override
    {
        texture_holder.draw(*this, world_position, custom_draw_size);
    }

    void draw_rectangle(glm::vec2 const world_position, glm::u64vec2 const size,
                        glm::u8vec3 const color) override
    {
        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, 255);
        glm::vec2 const screen_position = this->world_to_screen_position(world_position);
        if (!this->is_visible(screen_position, size))
        {
            return;
        }

        SDL_Rect const rect = {(int)screen_position.x, (int)screen_position.y, (int)size.x,
                               (int)size.y};
        SDL_RenderFillRect(this->renderer, &rect);
    }

    void draw_line(glm::u64vec2 const start, glm::u64vec2 const end,
                   glm::u8vec3 const color) override
    {
        // TODO: culling
        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, 255);
        SDL_RenderDrawLine(this->renderer, (int)start.x, (int)start.y, (int)end.x, (int)end.y);
    }

    void start_frame() override
    {
        this->clear({0, 0, 0});
    }

    void present() override
    {
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), this->renderer);
        SDL_RenderPresent(this->renderer);
    }

private:

    SDLRScreenRendererTexture load_sdl_texture(std::string_view const path)
    {
        SDL_Texture* const texture = IMG_LoadTexture(this->renderer, path.data());
        ib_runtime_assert(texture != nullptr, "Failed to create SDL texture");

        return SDLRScreenRendererTexture{texture};
    }
};
