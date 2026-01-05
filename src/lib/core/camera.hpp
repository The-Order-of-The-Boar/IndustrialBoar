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
    void update(double const delta, FrameInput const& frame_input);

    glm::vec2 get_position() const;
    glm::u64vec2 get_mouse_index() const;


private:

    glm::vec2 position{0, 0};
    glm::u64vec2 mouse_index;
};