#include "camera.hpp"

#include "constants.hpp"


void Camera::update(double const delta, FrameInput const& frame_input)
{
    for (auto const& event: frame_input.key_inputs)
    {
        switch (event.type)
        {
            case (InputEventType::UP):
                this->position.y += delta * Camera::CAMERA_SPEED;
                break;
            case (InputEventType::DOWN):
                this->position.y -= delta * Camera::CAMERA_SPEED;
                break;
            case (InputEventType::RIGHT):
                this->position.x -= delta * Camera::CAMERA_SPEED;
                break;
            case (InputEventType::LEFT):
                this->position.x += delta * Camera::CAMERA_SPEED;
                break;

            default:
                break;
        }
    }

    this->mouse_index =
        (frame_input.mouse_input.screen_position - static_cast<glm::u64vec2>(this->position)) /
        Constants::TILE_SIZE_VEC;
}

glm::vec2 Camera::get_position() const
{
    return this->position;
}

glm::u64vec2 Camera::get_mouse_index() const
{
    return this->mouse_index;
}