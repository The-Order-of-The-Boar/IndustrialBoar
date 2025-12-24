#pragma once

// builtin
#include <stdint.h>

// external
#include <glm/vec2.hpp>

struct Constants
{
    static constexpr glm::uvec2 SCREEN_SIZE{1920, 1024};
    static constexpr uint64_t const WORLD_SIZE = 128;
    static constexpr uint64_t const TILE_SIZE  = 64;
};
