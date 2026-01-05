#pragma once

// local
#include "../graphics/screen_renderer.hpp"
#include "building.hpp"
#include "resource.hpp"

class Belt : public Building
{
public:

    Belt(size_t const id, glm::u64vec2 const index, Rotation const rotation = Rotation::UP);

    std::optional<Building*> queued_to_move;

    void render(ScreenRenderer& renderer) const override;
};