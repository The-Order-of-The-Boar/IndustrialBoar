#pragma once

#include "building.hpp"

#include "../graphics/screen_renderer.hpp"

class Tile
{
public:

    enum GroundType
    {
        WATER,
        SAND,
        GRASS,
        STONE,
        COOPER,
        IRON
    };

    GroundType ground;
    BuildingRef building;

    TextureIDHolder const& get_texture() const;
};