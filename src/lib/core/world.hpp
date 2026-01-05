#pragma once

// builtin
#include <array>

// local
#include "../entities/belt.hpp"
#include "../entities/building.hpp"
#include "../entities/mine.hpp"
#include "../entities/tile.hpp"


class World
{
public:

    std::array<std::array<Tile, Constants::WORLD_SIZE>, Constants::WORLD_SIZE> tiles;

    std::tuple<std::vector<Belt>, std::vector<Mine>> buildings;


    Tile& get_tile(glm::u64vec2 const index)
    {
        return this->tiles[index.x][index.y];
    }

    Tile const& get_tile(glm::u64vec2 const index) const
    {
        return this->tiles[index.x][index.y];
    }

    template <typename BuildingT>
    BuildingT& get_building(size_t const id)
    {
        return this->get_buildings<BuildingT>().at(id);
    }

    template <typename BuildingT>
    BuildingT& get_building(glm::u64vec2 const index)
    {
        Tile& tile = this->get_tile(index);
        return this->get_building<BuildingT>(tile);
    }

    template <typename BuildingT>
    void add_building(BuildingT* building)
    {
        auto& building_vector = this->get_buildings<BuildingT>();
        size_t const new_id   = building_vector.size();
        building_vector.push_back(*building);
        auto& new_building = building_vector[new_id];
        new_building.id    = new_id;

        this->get_tile(building->world_index).building = new_building.get_ref();
    }

    template <typename BuildingT>
    std::vector<BuildingT> const& get_buildings() const
    {
        return std::get<std::vector<BuildingT>>(this->buildings);
    }

    template <typename BuildingT>
    std::vector<BuildingT>& get_buildings()
    {
        return std::get<std::vector<BuildingT>>(this->buildings);
    }
};
