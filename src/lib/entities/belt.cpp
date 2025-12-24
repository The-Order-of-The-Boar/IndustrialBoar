// header
#include "belt.hpp"

// builtin
#include <array>

// local
#include "../core/constants.hpp"

static inline std::array<TextureIDHolder, 4> textures{
    TextureIDHolder{"assets/textures/belt_up.png"},
    TextureIDHolder{"assets/textures/belt_right.png"},
    TextureIDHolder{"assets/textures/belt_down.png"},
    TextureIDHolder{"assets/textures/belt_left.png"},
};

Belt::Belt(size_t const id, glm::u64vec2 const index, Rotation const rotation):
    Building{index, BuildingType::BELT, id}
{
    this->current_rotation = rotation;
}

void Belt::render(ScreenRenderer& renderer) const
{
    TextureIDHolder const& texture = textures[current_rotation];
    renderer.draw_texture(
        texture,
        {this->world_index.x * Constants::TILE_SIZE, this->world_index.y * Constants::TILE_SIZE},
        std::make_optional<glm::u64vec2>(Constants::TILE_SIZE, Constants::TILE_SIZE));

    if (this->resource.has_value())
        this->resource->render(renderer);
}

glm::u64vec2 Belt::get_output_index() const
{
    switch (this->current_rotation)
    {
        case Rotation::UP:
            return {this->world_index.x, this->world_index.y - 1};
        case Rotation::RIGHT:
            return {this->world_index.x + 1, this->world_index.y};
        case Rotation::DOWN:
            return {this->world_index.x, this->world_index.y + 1};
        case Rotation::LEFT:
            return {this->world_index.x - 1, this->world_index.y};

        default:
            return {};
    }
}
