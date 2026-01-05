#pragma once

// third party
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// local
#include <SDL_render.h>

// builtin
#include <optional>
#include <string>

struct TextureID
{
    size_t const id;
};

struct SDLRendererTexture
{
public:

    SDL_Texture* texture = nullptr;

public: // ctor's, dtor's, and move semantics

    explicit SDLRendererTexture(SDL_Texture* const texture);
    SDLRendererTexture(SDLRendererTexture const&)            = delete;
    SDLRendererTexture& operator=(SDLRendererTexture const&) = delete;

    SDLRendererTexture(SDLRendererTexture&& other) noexcept;

    SDLRendererTexture& operator=(SDLRendererTexture&& other) noexcept;

    ~SDLRendererTexture();

public: // public methods

    [[nodiscard]] glm::u64vec2 get_size() const;
};
class TextureIDHolder
{
public:

    std::string const path;
    mutable std::optional<TextureID> texture_id;
    glm::u8vec3 modulate;

    void set_modulate(glm::u8vec3 const color);

    explicit TextureIDHolder(std::string _path);
};