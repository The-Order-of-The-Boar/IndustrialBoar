#pragma once

// builtin
#include <cstddef>
#include <optional>
#include <string_view>

// external
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>



class TextureIDHolder;

struct TextureID
{
    size_t const id;
};

class ScreenRenderer
{
public:

    virtual TextureID load_texture(std::string_view path) = 0;

    virtual void clear(glm::u8vec3 clear_color = {0, 0, 0}) = 0;
    virtual void draw_texture(TextureID texture, glm::u64vec2 position,
                              std::optional<glm::u64vec2> custom_draw_size = std::nullopt) = 0;
    virtual void draw_texture(TextureIDHolder const& texture_holder, glm::u64vec2 position,
                              std::optional<glm::u64vec2> custom_draw_size = std::nullopt) = 0;
    virtual void draw_rectangle(glm::u64vec2 position, glm::u64vec2 size, glm::u8vec3 color) = 0;
    virtual void draw_line(glm::u64vec2 start, glm::u64vec2 end, glm::u8vec3 color) = 0;

    virtual void start_frame() = 0;
    virtual void present() = 0;

    virtual ~ScreenRenderer() = default;
};


class TextureIDHolder
{
private:

    std::string const path;
    mutable std::optional<TextureID> texture_id;

public:

    explicit TextureIDHolder(std::string _path): path(std::move(_path)) {}

    void draw(ScreenRenderer& renderer, glm::u64vec2 position,
              std::optional<glm::u64vec2> custom_draw_size = std::nullopt) const
    {
        if (this->texture_id.has_value() == false)
            this->texture_id.emplace(renderer.load_texture(this->path));

        renderer.draw_texture(this->texture_id.value(), position, custom_draw_size);
    }
};
