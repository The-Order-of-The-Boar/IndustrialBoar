#pragma once


// local
#include "../core/entity_manager.hpp"
#include "../utils/assert.hpp"
#include "./screen_renderer.hpp"



class GraphicEntity : public Entity
{
public:

    GraphicEntity(const glm::u64vec2 index): world_index{index} {};

    glm::u64vec2 world_index;

    virtual void render(ScreenRenderer& renderer) const = 0;

    [[nodiscard]] bool is_child_guard_public() const override
    {
        return false;
    };
};