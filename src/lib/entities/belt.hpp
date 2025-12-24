#pragma once

#include <iostream>

// local
#include "../graphics/screen_renderer.hpp"
#include "building.hpp"
#include "resource.hpp"

class Belt : public Building
{
public:

    Belt(size_t const id, glm::u64vec2 const index, Rotation const rotation);

    std::optional<Resource> queued_resource;
    std::optional<Resource> resource;

    void render(ScreenRenderer& renderer) const override;

    glm::u64vec2 get_output_index() const;
};