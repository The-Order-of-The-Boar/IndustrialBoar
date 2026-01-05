#pragma once

#include <array>

#include "../core/perlim_noise.hpp"
#include "../entities/tile.hpp"
#include "constants.hpp"
#include "world.hpp"


class WorldGenerator
{
public:

    static World generate();
};
