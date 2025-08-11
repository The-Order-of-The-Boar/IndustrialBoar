#pragma once

#include <iostream>
// builtin
#include <optional>

// local
#include "../core/constants.hpp"
#include "../graphics/graphic_entity.hpp"
#include "../graphics/screen_renderer.hpp"


class Resource : public GraphicEntity
{
    static const constexpr uint64_t ITEM_SIZE = Constants::TILE_SIZE / 2;
    static const constexpr std::optional<glm::u64vec2> RENDER_SIZE =
        glm::u64vec2(ITEM_SIZE, ITEM_SIZE);

public:

    enum ResourceType
    {
        IRON = 0,
        COOPER
    };

    Resource(const glm::u64vec2 index, const ResourceType type);

    ResourceType type;
    bool moved_this_tick = false;

    void render(ScreenRenderer& renderer) const override;
};