#include "camera.hpp"


void Camera::update(double const delta, std::vector<InputEvent> const& input_events)
{
    for (auto const& event: input_events)
    {
        switch (event.type)
        {
            case (InputEventType::UP):
                this->position.y -= delta * Camera::CAMERA_SPEED;
                break;
            case (InputEventType::DOWN):
                this->position.y += delta * Camera::CAMERA_SPEED;
                break;
            case (InputEventType::RIGHT):
                this->position.x += delta * Camera::CAMERA_SPEED;
                break;
            case (InputEventType::LEFT):
                this->position.x -= delta * Camera::CAMERA_SPEED;
                break;

            default:
                break;
        }
    }
}

glm::vec2 Camera::get_position() const
{
    return this->position;
}