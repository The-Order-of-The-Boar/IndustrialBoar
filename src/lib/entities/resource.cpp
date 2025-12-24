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

Resource::Resource(glm::u64vec2 const index, ResourceType const type):
    world_index{index}, type{type}
{
}

void Resource::render(ScreenRenderer& renderer) const
{
    TextureIDHolder const& texture  = textures[this->type];
    uint64_t const tile_draw_offset = (Constants::TILE_SIZE / 2) - (Resource::ITEM_SIZE / 2);
    renderer.draw_texture(texture,
                          {this->world_index.x * Constants::TILE_SIZE + tile_draw_offset,
                           this->world_index.y * Constants::TILE_SIZE + tile_draw_offset},
                          Resource::RENDER_SIZE);
}
