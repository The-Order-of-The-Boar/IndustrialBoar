#pragma once

// builtin
#include <cstddef>
#include <optional>

// local

// external
#include <glm/ext/vector_uint2_sized.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string_view>



struct TextureID {
    size_t const id;
};

class ScreenRenderer {
public:

    virtual TextureID load_texture(std::string_view const path) = 0;
    
    virtual void clear(glm::u8vec3 const clear_color = {0, 0, 0}) = 0;
    virtual void draw_texture(TextureID const texture, glm::u64vec2 const position, std::optional<glm::u64vec2> custom_draw_size = std::nullopt) = 0;
    virtual void draw_rectangle(glm::u64vec2 const position, glm::u64vec2 const size, glm::u8vec3 const color) = 0;
    virtual void draw_line(glm::u64vec2 const start, glm::u64vec2 const end, glm::u8vec3 const color) = 0;
    
    virtual void start_frame() = 0;
    virtual void present() = 0;
    
    virtual ~ScreenRenderer() = default;
};
