#pragma once

// local
#include "../entities/building.hpp"
#include "../entities/managers/construction_manager.hpp"
#include "../graphics/imgui_handler.hpp"


struct ConstructionData
{
    std::string menu_name;
    TextureIDHolder texture;
    BuildingType building_type;

    ConstructionData(std::string const& menu_name, std::string const& texture_path,
                     BuildingType const building_type):
        menu_name{menu_name}, texture{texture_path}, building_type{building_type}
    {
    }
};

class BuildMenu
{
private:

    ConstructionManager& construction_manager;

    static constexpr std::optional<glm::u64vec2> const ICON_SIZE =
        std::make_optional<glm::u64vec2>(64, 64);

public:

    BuildMenu(ConstructionManager& construction_manager);

    void render_hud(ImGuiHandler& hud_handler) const;
};