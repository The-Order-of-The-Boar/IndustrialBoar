#pragma once

// builtin
#include <glm/ext/vector_uint2_sized.hpp>
#include <string_view>
#include <unordered_map>

// local
#include "../utils/assert.hpp"
#include "screen_renderer.hpp"

// external
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <fmt/format.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// TODO: support HIDPI displays



struct SDLRScreenRendererTexture
{
public:

    SDL_Texture* texture = nullptr;

public: // ctor's, dtor's, and move semantics

    explicit SDLRScreenRendererTexture(SDL_Texture* const texture): texture{texture} {}
    SDLRScreenRendererTexture(SDLRScreenRendererTexture const&) = delete;
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
private:

    SDL_Renderer* renderer = nullptr;
    size_t next_texture_id = 0;
    std::unordered_map<size_t, SDLRScreenRendererTexture> textures;

public:

    explicit SDLRScreenRenderer(SDL_Window* window)
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


    TextureID load_texture(const std::string_view path) override
    {
        SDLRScreenRendererTexture texture = this->load_sdl_texture(path);
        this->textures.insert({this->next_texture_id, std::move(texture)});
        return TextureID{this->next_texture_id++};
    }

    void clear(const glm::u8vec3 clear_color) override
    {
        SDL_SetRenderDrawColor(this->renderer, clear_color.r, clear_color.g, clear_color.b, 255);
        SDL_RenderClear(this->renderer);
    }

    void draw_texture(TextureID const texture, const glm::u64vec2 position,
                      std::optional<glm::u64vec2> custom_draw_size = std::nullopt) override
    {
        SDLRScreenRendererTexture const& sdl_texture = this->textures.at(texture.id);
        glm::u64vec2 const draw_size = custom_draw_size.value_or(sdl_texture.get_size());

        SDL_Rect const dest_rect = {(int)position.x, (int)position.y, (int)draw_size.x,
                                    (int)draw_size.y};

        SDL_RenderCopy(this->renderer, sdl_texture.texture, nullptr, &dest_rect);
    }
    void draw_texture(TextureIDHolder const& texture_holder, const glm::u64vec2 position,
                      std::optional<glm::u64vec2> custom_draw_size = std::nullopt) override
    {
        texture_holder.draw(*this, position, custom_draw_size);
    }

    void draw_rectangle(const glm::u64vec2 position, const glm::u64vec2 size,
                        const glm::u8vec3 color) override
    {
        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, 255);
        SDL_Rect const rect = {(int)position.x, (int)position.y, (int)size.x, (int)size.y};
        SDL_RenderFillRect(this->renderer, &rect);
    }

    void draw_line(const glm::u64vec2 start, const glm::u64vec2 end,
                   const glm::u8vec3 color) override
    {
        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, 255);
        SDL_RenderDrawLine(this->renderer, (int)start.x, (int)start.y, (int)end.x, (int)end.y);
    }

    void start_frame() override
    {
        this->clear({0, 0, 0});
    }

    void present() override
    {
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
