#include "tile.hpp"

#include <array>

static inline std::array<TextureIDHolder, 6> textures{
    TextureIDHolder{"assets/textures/tiles/water.png"},
    TextureIDHolder{"assets/textures/tiles/sand.png"},
    TextureIDHolder{"assets/textures/tiles/grass.png"},
    TextureIDHolder{"assets/textures/tiles/stone.png"},
    TextureIDHolder{"assets/textures/tiles/copper.png"},
    TextureIDHolder{"assets/textures/tiles/iron.png"},
};



TextureIDHolder const& Tile::get_texture() const
{
    return textures[this->ground];
}