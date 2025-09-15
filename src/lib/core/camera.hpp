#pragma once

// builtin
#include <vector>

// external
#include <glm/vec2.hpp>

// local
#include "input.hpp"



class Camera
{
public:

    static double const constexpr CAMERA_SPEED = 100;
    void update(double const delta, std::vector<InputEvent> const& input_events);

    glm::vec2 get_position() const;


private:

    glm::vec2 position{0, 0};
};