#include "texture.hpp"

#include <fmt/format.h>

#include "../utils/assert.hpp"

SDLRendererTexture::SDLRendererTexture(SDL_Texture* const texture): texture{texture} {}
SDLRendererTexture::SDLRendererTexture(SDLRendererTexture&& other) noexcept: texture{other.texture}
{
    other.texture = nullptr;
}

SDLRendererTexture& SDLRendererTexture::operator=(SDLRendererTexture&& other) noexcept
{
    if (this != &other)
    {
        this->texture = other.texture;
        other.texture = nullptr;
    }
    return *this;
}
SDLRendererTexture::~SDLRendererTexture()
{
    if (this->texture == nullptr)
        return;

    SDL_DestroyTexture(this->texture);
}

[[nodiscard]] glm::u64vec2 SDLRendererTexture::get_size() const
{
    int width, height;
    int result = SDL_QueryTexture(this->texture, nullptr, nullptr, &width, &height);
    ib_runtime_assert(result == 0,
                      fmt::format("Failed to query SDL texture size, {}", SDL_GetError()));
    return glm::u64vec2{(uint64_t)width, (uint64_t)height};
}

TextureIDHolder::TextureIDHolder(std::string _path): path(std::move(_path)) {}

void TextureIDHolder::set_modulate(glm::u8vec3 const color)
{
    this->modulate = color;
}
