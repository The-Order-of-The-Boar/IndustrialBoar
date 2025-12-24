#pragma once

// local
#include "../graphics/screen_renderer.hpp"

enum BuildingType
{
    NO_BUILDING = 0,
    BELT,
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

    Building(glm::u64vec2 const index, BuildingType const building_type, size_t const id):
        world_index{index}, building_type{building_type}, id{id} {};

    BuildingRef get_ref() const
    {
        BuildingRef ref;
        ref.type = building_type;
        ref.id   = id;
        return ref;
    }


    virtual void render(ScreenRenderer& renderer) const = 0;
};