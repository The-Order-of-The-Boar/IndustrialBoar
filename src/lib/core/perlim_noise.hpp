#pragma once

// builtin
#include <array>
#include <stdint.h>

// external
#include <glm/vec2.hpp>


class PerlimNoise
{
public:

    PerlimNoise(uint32_t const seed = 666);
    void regenerate(uint32_t const seed);

    double get_noise_2d(glm::dvec2 const point) const;
    double get_fractal_brownian_motion_2d(glm::dvec2 const point, uint64_t const octets,
                                          double frequency = 0.05) const;

private:

    glm::dvec2 get_vector_from_index(int64_t const index) const;
    std::array<int64_t, 512> permutation_table;
};
