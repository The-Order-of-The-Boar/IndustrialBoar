// header
#include "mine.hpp"

// builtin
#include <array>

// local
#include "../core/constants.hpp"

static inline std::array<TextureIDHolder, 4> textures{
    TextureIDHolder{"assets/textures/buildings/mine_up.png"},
    TextureIDHolder{"assets/textures/buildings/mine_right.png"},
    TextureIDHolder{"assets/textures/buildings/mine_down.png"},
    TextureIDHolder{"assets/textures/buildings/mine_left.png"},
};

Mine::Mine(size_t const id, glm::u64vec2 const index, Rotation const rotation,
           Resource::ResourceType const resource_type):
    Building{index, BuildingType::MINE, id}, mine_resource{index, resource_type}
{
    this->resource         = std::make_optional<Resource>(index, resource_type);
    this->current_rotation = rotation;
}

void Mine::render(ScreenRenderer& renderer) const
{
    TextureIDHolder const& texture = textures[current_rotation];
    renderer.draw_texture(
        texture,
        {this->world_index.x * Constants::TILE_SIZE, this->world_index.y * Constants::TILE_SIZE},
        std::make_optional<glm::u64vec2>(Constants::TILE_SIZE, Constants::TILE_SIZE));

    // if (this->resource.has_value())
    //     this->resource->render(renderer);
}
