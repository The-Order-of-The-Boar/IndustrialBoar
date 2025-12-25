#pragma once

// local
#include "../graphics/screen_renderer.hpp"
#include "resource.hpp"

enum BuildingType
{
    NO_BUILDING = 0,
    BELT,
    MINE,
};


class BuildingRef
{
public:

    BuildingType type = BuildingType::NO_BUILDING;
    size_t id;
};

class Building
{
public:

    enum Rotation
    {
        UP = 0,
        RIGHT,
        DOWN,
        LEFT
    };

    glm::u64vec2 world_index;
    Rotation current_rotation;
    BuildingType const building_type;
    size_t const id;
    std::optional<Resource> resource;

    Building(glm::u64vec2 const index, BuildingType const building_type, size_t const id):
        world_index{index}, building_type{building_type}, id{id} {};

    BuildingRef get_ref() const
    {
        BuildingRef ref;
        ref.type = building_type;
        ref.id   = id;
        return ref;
    }

    glm::u64vec2 get_output_index() const
    {
        switch (this->current_rotation)
        {
            case Rotation::UP:
                return {this->world_index.x, this->world_index.y - 1};
            case Rotation::RIGHT:
                return {this->world_index.x + 1, this->world_index.y};
            case Rotation::DOWN:
                return {this->world_index.x, this->world_index.y + 1};
            case Rotation::LEFT:
                return {this->world_index.x - 1, this->world_index.y};

            default:
                return {};
        }
    }


    virtual void render(ScreenRenderer& renderer) const = 0;
};