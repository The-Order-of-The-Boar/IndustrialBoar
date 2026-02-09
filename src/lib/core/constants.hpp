#pragma once

// builtin
#include <stdint.h>

// external
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Constants
{
    static constexpr glm::u64vec2 SCREEN_SIZE{1920, 1024};
    static constexpr uint64_t const WORLD_SIZE        = 128;
    static constexpr uint64_t const TILE_SIZE         = 64;
    static constexpr uint64_t const RENDER_SCALE      = 2;
    static constexpr glm::u64vec2 const TILE_SIZE_VEC = glm::u64vec2{TILE_SIZE, TILE_SIZE};

    static constexpr double TICK_SECONDS = 1;

    static constexpr glm::u8vec3 WHITE{255, 255, 255};

    static constexpr glm::vec2 ZERO_ANGLE_VECTOR{0, -1};
};
