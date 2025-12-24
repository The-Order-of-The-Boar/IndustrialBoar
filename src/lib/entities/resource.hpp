#pragma once

#include <iostream>
// builtin
#include <optional>

// local
#include "../core/constants.hpp"
#include "../graphics/screen_renderer.hpp"


class Resource
{
    static constexpr uint64_t const ITEM_SIZE = Constants::TILE_SIZE / 2;
    static constexpr std::optional<glm::u64vec2> const RENDER_SIZE =
        glm::u64vec2(ITEM_SIZE, ITEM_SIZE);

public:

    enum ResourceType
    {
        IRON = 0,
        COOPER
    };

    Resource(glm::u64vec2 const index, ResourceType const type);

    glm::u64vec2 world_index;
    ResourceType type;
    bool moved_this_tick = false;

    void render(ScreenRenderer& renderer) const;
};