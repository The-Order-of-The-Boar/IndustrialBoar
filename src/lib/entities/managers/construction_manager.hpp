#pragma once

#include "../../core/world.hpp"
#include "../belt.hpp"
#include "../building.hpp"
#include "../mine.hpp"

class ConstructionManager
{
private:

    static constexpr glm::u8vec3 const CANNOT_BUILD_COLOR{255, 64, 64};

    World& world;
    std::unique_ptr<Building> selected_building;


public:

    ConstructionManager(World& world);

    void set_selected_building(BuildingType const building_type);

    void update(double delta, FrameInput const& frame_input);
    void render(ScreenRenderer& renderer) const;
};
