#include "math.hpp"

#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../core/constants.hpp"

double Math::lerp(double const t, double const start, double const end)
{
    ib_debug_assert(t >= 0 && t <= 1.0, "Non-normalized t argument");
    return start + t * (end - start);
}

double Math::fade(double const t)
{
    return ((6 * t - 15) * t + 10) * t * t * t;
}

glm::vec2 Math::get_direction_vector(double const rotation_deg)
{
    double const rotation_rad = glm::radians(rotation_deg);
    return glm::vec2{std::cos(rotation_rad), std::sin(rotation_rad)};
}

double Math::get_vector_rotation_deg(glm::vec2 const vec)
{
    double const target_angle_rad =
        glm::orientedAngle(Constants::ZERO_ANGLE_VECTOR, glm::normalize(vec));
    return glm::degrees(target_angle_rad);
}