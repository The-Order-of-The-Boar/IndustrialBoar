#pragma once

#include <iostream>

// local
#include "../graphics/graphic_entity.hpp"
#include "../graphics/screen_renderer.hpp"
#include "resource.hpp"

class Belt : public GraphicEntity
{
public:

    enum Rotation
    {
        UP = 0,
        RIGHT,
        DOWN,
        LEFT
    };

    Belt(glm::u64vec2 const index, Rotation const rotation);

    Rotation current_rotation;
    std::optional<Resource> queued_resource;
    std::optional<Resource> resource;

    void render(ScreenRenderer& renderer) const override;

    glm::u64vec2 get_output_index() const;
};