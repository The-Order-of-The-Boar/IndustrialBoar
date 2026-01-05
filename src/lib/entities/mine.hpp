#pragma once

// local
#include "../graphics/screen_renderer.hpp"
#include "building.hpp"
#include "resource.hpp"

class Mine : public Building
{
public:

    Mine(size_t const id, glm::u64vec2 const index, Rotation const rotation,
         Resource::ResourceType const resource_type);

    Resource mine_resource;

    void render(ScreenRenderer& renderer) const override;
    bool can_be_build_at(Tile const& tile) const override;
};