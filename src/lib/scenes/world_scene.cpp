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

WorldScene::WorldScene()
{
    WorldGenerator::generate(this->world);

    this->mines.emplace_back(0, glm::u64vec2{2, 0}, Building::Rotation::DOWN,
                             Resource::ResourceType::COOPER);
    this->world[2][0].building = this->mines[0].get_ref();

    for (int64_t i = 0; i < 3; ++i)
    {
        belts.emplace_back(this->belts.size(), glm::u64vec2{2, i + 1}, Belt::Rotation::DOWN);
        this->world[2][i + 1].building = this->belts.at(this->belts.size() - 1).get_ref();
    }

    for (int64_t i = -2; i < 4; ++i)
    {
        belts.emplace_back(this->belts.size(), glm::u64vec2{4 + i, 4}, Belt::Rotation::RIGHT);
        this->world[4 + i][4].building = this->belts.at(this->belts.size() - 1).get_ref();
    }
    Belt* b     = &this->belts.at(5);
    b->resource = Resource{b->world_index, Resource::ResourceType::COOPER};
    for (int64_t i = 0; i < 4; ++i)
    {
        belts.emplace_back(this->belts.size(), glm::u64vec2{4 + 4, 4 + i}, Belt::Rotation::DOWN);
        this->world[4 + 4][4 + i].building = this->belts.at(this->belts.size() - 1).get_ref();
    }
    b           = &this->belts.at(4);
    b->resource = Resource{b->world_index, Resource::ResourceType::IRON};

    for (int64_t i = 0; i < 4; ++i)
    {
        belts.emplace_back(this->belts.size(), glm::u64vec2{4 + 4 - i, 4 + 4},
                           Belt::Rotation::LEFT);
        this->world[4 + 4 - i][4 + 4].building = this->belts.at(this->belts.size() - 1).get_ref();
    }
}

void WorldScene::try_move_resource(Building& origin)
{
    glm::u64vec2 const target_index = origin.get_output_index();
    if (this->world[target_index.x][target_index.y].building.type == BuildingType::BELT)
    {
        Belt& target_building =
            this->belts.at(this->world[target_index.x][target_index.y].building.id);
        if (!target_building.resource.has_value())
        {
            target_building.resource                  = origin.resource;
            target_building.resource->world_index     = target_index;
            target_building.resource->moved_this_tick = true;
            origin.resource                           = std::nullopt;
            if (origin.building_type == BuildingType::BELT)
            {
                Belt& origin_belt = this->belts.at(origin.id);
                if (origin_belt.queued_to_move.has_value())
                {
                    try_move_resource(*origin_belt.queued_to_move.value());
                    origin_belt.queued_to_move = std::nullopt;
                }
            }
        }
        else
        {
            target_building.queued_to_move.emplace(&origin);
        }
    }
}

void WorldScene::update_belts()
{
    for (Belt& belt: this->belts)
    {
        if (belt.resource.has_value())
            belt.resource->moved_this_tick = false;
        if (belt.queued_to_move.has_value())
            belt.queued_to_move = std::nullopt;
    }

    for (Belt& belt: this->belts)
    {
        if (belt.resource.has_value() && !belt.resource->moved_this_tick)
        {
            this->try_move_resource(belt);
        }
    }
}

void WorldScene::update_mines()
{
    for (Mine& mine: this->mines)
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


std::optional<SceneExit> WorldScene::update(double delta, std::vector<InputEvent> input_events,
                                            SceneGroup& scene_group)
{
    (void)delta;
    (void)input_events;
    (void)scene_group;

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
            renderer.draw_texture(this->world[x][y].get_texture(), tile_size * glm::vec2{x, y},
                                  tile_size);
        }
    }

    for (Belt const& belt: this->belts)
    {
        belt.render(renderer);
    }

    for (Mine const& mine: this->mines)
    {
        mine.render(renderer);
    }

    // frame.print_time();
}

void WorldScene::render_hud([[maybe_unused]] ImGuiHandler& renderer) const
{
    bool panel_visible = true;
    ImGui::Begin("Game", &panel_visible);
    ImGui::Text("Boar Colonization Started");
    ImGui::ShowDemoWindow();

    auto const io = ImGui::GetIO();

    ImGui::End();
}
