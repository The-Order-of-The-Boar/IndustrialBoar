#pragma once

// local
#include "assert.hpp"

#include "glm/vec2.hpp"


namespace Math
{
    double lerp(double const t, double const start, double const end);

    double fade(double const t);

    glm::vec2 get_direction_vector(double const rotation);
    double get_vector_rotation_deg(glm::vec2 const vec);

} // namespace Math
