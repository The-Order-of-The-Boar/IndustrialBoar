#pragma once

// local
#include "../graphics/screen_renderer.hpp"
#include "building_ref.hpp"
#include "resource.hpp"
#include "tile.hpp"

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
    glm::u8vec3 modulate = Constants::WHITE;
    Rotation current_rotation;
    BuildingType const building_type;
    size_t id;
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

    void rotate(int64_t const amount)
    {
        this->current_rotation =
            static_cast<Building::Rotation>((this->current_rotation + amount + 4) % 4);
    }

    void rotate_left()
    {
        this->rotate(-1);
    }

    void rotate_right()
    {
        this->rotate(1);
    }

    virtual bool can_be_build_at(Tile const& tile) const
    {
        return tile.building.id == BuildingType::NO_BUILDING;
    }

    virtual void render(ScreenRenderer& renderer) const = 0;
};