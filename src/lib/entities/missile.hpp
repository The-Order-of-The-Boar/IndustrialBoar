#pragma once

// local
#include "../graphics/screen_renderer.hpp"

class Missile
{
public:

    static double const constexpr ROTATION_SPEED = 90;
    static double constexpr const MAX_SPEED      = 100;
    static double constexpr const ACCELERATION   = 10;


    size_t id;
    double rotation;
    double speed;
    glm::vec2 position{100, 100};
    glm::vec2 target_position;

    void update(double const delta);
    void render(ScreenRenderer& renderer) const;
};