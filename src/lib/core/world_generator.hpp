#pragma once

#include <array>

#include "../core/perlim_noise.hpp"
#include "../entities/tile.hpp"
#include "constants.hpp"


class WorldGenerator
{
public:

    static void generate(
        std::array<std::array<Tile, Constants::WORLD_SIZE>, Constants::WORLD_SIZE>& world);
};
