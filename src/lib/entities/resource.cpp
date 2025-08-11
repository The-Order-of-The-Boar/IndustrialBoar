// header
#include "resource.hpp"

// builtin
#include <array>

// local
#include "../core/constants.hpp"

static inline std::array<TextureIDHolder, 2> textures{
    TextureIDHolder{"assets/textures/iron.png"},
    TextureIDHolder{"assets/textures/cooper.png"},
};

Resource::Resource(const glm::u64vec2 index, const ResourceType type):
    GraphicEntity{index}, type{type}
{
}

void Resource::render(ScreenRenderer& renderer) const
{
    TextureIDHolder const& texture = textures[this->type];
    const uint64_t tile_draw_offset = (Constants::TILE_SIZE / 2) - (Resource::ITEM_SIZE / 2);
    renderer.draw_texture(texture,
                          {this->world_index.x * Constants::TILE_SIZE + tile_draw_offset,
                           this->world_index.y * Constants::TILE_SIZE + tile_draw_offset},
                          Resource::RENDER_SIZE);
}
