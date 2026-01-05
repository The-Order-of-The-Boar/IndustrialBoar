// header
#include "world_scene.hpp"


// builint
#include <iomanip>
#include <iostream>

// external
#include <imgui.h>

// local
#include "../core/perlim_noise.hpp"
#include "../core/world_generator.hpp"
#include "../entities/belt.hpp"
#include "../utils/time_utils.hpp"

WorldScene::WorldScene():
    world{WorldGenerator::generate()}, construction_manager{world},
    build_menu{this->construction_manager}
{
}

void WorldScene::try_move_resource(Building& origin)
{
    glm::u64vec2 target_index = origin.get_output_index();
    Tile& target_tile         = this->world.get_tile(target_index);
    if (target_tile.building.type == BuildingType::BELT)
    {
        Belt& target_belt = this->world.get_building<Belt>(target_tile.building.id);
        if (!target_belt.resource.has_value())
        {
            target_belt.resource                  = origin.resource;
            target_belt.resource->world_index     = target_index;
            target_belt.resource->moved_this_tick = true;
            origin.resource                       = std::nullopt;
            if (origin.building_type == BuildingType::BELT)
            {
                Belt& origin_belt = this->world.get_building<Belt>(origin.id);
                if (origin_belt.queued_to_move.has_value())
                {
                    try_move_resource(*origin_belt.queued_to_move.value());
                    origin_belt.queued_to_move = std::nullopt;
                }
            }
        }
        else
        {
            target_belt.queued_to_move.emplace(&origin);
        }
    }
}

void WorldScene::update_belts()
{
    for (Belt& belt: this->world.get_buildings<Belt>())
    {
        if (belt.resource.has_value())
            belt.resource->moved_this_tick = false;
        if (belt.queued_to_move.has_value())
            belt.queued_to_move = std::nullopt;
    }

    for (Belt& belt: this->world.get_buildings<Belt>())
    {
        if (belt.resource.has_value() && !belt.resource->moved_this_tick)
        {
            this->try_move_resource(belt);
        }
    }
}

void WorldScene::update_mines()
{
    for (Mine& mine: this->world.get_buildings<Mine>())
    {
        mine.resource = mine.mine_resource;
        this->try_move_resource(mine);
    }
}


void WorldScene::tick()
{
    this->update_belts();
    this->update_mines();
}


std::optional<SceneExit> WorldScene::update(double delta, FrameInput const& frame_input,
                                            SceneGroup& scene_group)
{
    (void)scene_group;

    this->construction_manager.update(delta, frame_input);

    return std::nullopt;
}

void WorldScene::render(ScreenRenderer& renderer) const
{
    TimeMeasurer frame{"render all time", TimeMeasurer::MILLISECOND};
    for (size_t y = 0; y < Constants::WORLD_SIZE; ++y)
    {
        for (size_t x = 0; x < Constants::WORLD_SIZE; ++x)
        {
            glm::vec2 const tile_size = glm::uvec2{Constants::TILE_SIZE, Constants::TILE_SIZE};
            renderer.draw_texture(this->world.get_tile({x, y}).get_texture(),
                                  tile_size * glm::vec2{x, y}, tile_size);
        }
    }

    for (Belt const& belt: this->world.get_buildings<Belt>())
    {
        belt.render(renderer);
    }

    for (Mine const& mine: this->world.get_buildings<Mine>())
    {
        mine.render(renderer);
    }

    this->construction_manager.render(renderer);

    // frame.print_time();
}

void WorldScene::render_hud([[maybe_unused]] ImGuiHandler& hud_handler) const
{
    bool panel_visible = true;
    ImGui::Begin("Game", &panel_visible);
    ImGui::Text("Boar Colonization Started");
    ImGui::ShowDemoWindow();

    this->build_menu.render_hud(hud_handler);

    ImGui::End();
}
