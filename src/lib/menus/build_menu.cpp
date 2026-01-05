#include "build_menu.hpp"

// builtin
#include <vector>

// external
#include <imgui.h>
#include <iostream>


std::vector<ConstructionData> buildings{
    {"Mine", "assets/textures/buildings/mine_up.png", BuildingType::MINE},
    {"Belt", "assets/textures/buildings/belt_up.png", BuildingType::BELT},
};

BuildMenu::BuildMenu(ConstructionManager& construction_manager):
    construction_manager{construction_manager}
{
}


void BuildMenu::render_hud(ImGuiHandler& hud_handler) const
{
    TextureIDHolder menu_texture{"assets/textures/buildings/mine_up.png"};


    ImGui::Begin("Build Menu");
    ImGui::BeginTable("Buildings", 2);
    for (auto& building: buildings)
    {
        ImGui::TableNextColumn();
        if (hud_handler.image_button(building.menu_name, building.texture, ICON_SIZE))
        {
            this->construction_manager.set_selected_building(building.building_type);
        }
    }
    ImGui::EndTable();
    ImGui::End();
}