// header
#include "perlim_noise.hpp"

// builtin
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

// external
#include <glm/glm.hpp>

// local
#include "../utils/math.hpp"

PerlimNoise::PerlimNoise(uint32_t const seed)
{
    this->regenerate(seed);
}

void PerlimNoise::regenerate(uint32_t const seed)
{
    for (int64_t i = 0; i < 256; i++)
        this->permutation_table[i] = i;

    std::shuffle(this->permutation_table.begin(), this->permutation_table.begin() + 256,
                 std::default_random_engine(seed));

    for (int64_t i = 0; i < 256; i++)
        this->permutation_table[256 + i] = this->permutation_table[i];
}

double PerlimNoise::get_noise_2d(glm::dvec2 const point) const
{
    int64_t const permutation_x = static_cast<int64_t>(std::floor(point.x)) % 255;
    int64_t const permutation_y = static_cast<int64_t>(std::floor(point.y)) % 255;

    glm::dvec2 const normalized{point.x - std::floor(point.x), point.y - std::floor(point.y)};
    // glm::dvec2 const normalized{0.5, 0.5};


    glm::dvec2 const top_right_distance    = normalized - glm::dvec2(1, 1); // 1
    glm::dvec2 const top_left_distance     = normalized - glm::dvec2(0, 1); // 2
    glm::dvec2 const bottom_right_distance = normalized - glm::dvec2(1, 0); // 3
    glm::dvec2 const bottom_left_distance  = normalized - glm::dvec2(0, 0); // 4

    glm::dvec2 const top_right_vector = this->get_vector_from_index(
        this->permutation_table[this->permutation_table[permutation_x + 1] + permutation_y + 1]);
    glm::dvec2 const top_left_vector = this->get_vector_from_index(
        this->permutation_table[this->permutation_table[permutation_x] + permutation_y + 1]);

    glm::dvec2 const bottom_right_vector = this->get_vector_from_index(
        this->permutation_table[this->permutation_table[permutation_x + 1] + permutation_y]);
    glm::dvec2 const bottom_left_vector = this->get_vector_from_index(
        this->permutation_table[this->permutation_table[permutation_x] + permutation_y]);

    double const top_left_value     = glm::dot(top_left_distance, top_left_vector);
    double const top_right_value    = glm::dot(top_right_distance, top_right_vector);
    double const bottom_right_value = glm::dot(bottom_right_distance, bottom_right_vector);
    double const bottom_left_value  = glm::dot(bottom_left_distance, bottom_left_vector);

    double const u = Math::fade(normalized.x);
    double const v = Math::fade(normalized.y);

    double const left_lerp  = Math::lerp(v, bottom_left_value, top_left_value);
    double const right_lerp = Math::lerp(v, bottom_right_value, top_right_value);
    double const noise      = Math::lerp(u, left_lerp, right_lerp);

    return noise;
}

double PerlimNoise::get_fractal_brownian_motion_2d(glm::dvec2 const point, uint64_t const octaves,
                                                   double frequency) const
{
    double noise           = 0.0;
    double total_amplitude = 0.0;
    double amplitude       = 1.0;

    for (size_t i = 0; i < octaves; i++)
    {
        glm::dvec2 sampled_point = point * glm::dvec2(frequency, frequency);
        double const raw_noise   = amplitude * this->get_noise_2d(sampled_point);
        noise += raw_noise;

        total_amplitude += amplitude;
        amplitude *= 0.5;
        frequency *= 2;
    }

    // TODO: better normalization
    noise = (noise / 2) + 0.5;
    if (noise > 1)
        noise = 1;
    else if (noise < 0)
        noise = 0;

    return noise;
}

glm::dvec2 PerlimNoise::get_vector_from_index(int64_t const index) const
{
    int64_t const normalized_index = index % 4;
    switch (normalized_index)
    {
        case 0:
            return glm::dvec2(-1, +1);
        case 1:
            return glm::dvec2(+1, +1);
        case 2:
            return glm::dvec2(-1, -1);
        case 3:
            return glm::dvec2(+1, -1);
    }
    return glm::dvec2(0, 0);
}