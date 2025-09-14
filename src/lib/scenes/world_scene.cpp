// header
#include "world_scene.hpp"
#include <iostream>

// local
#include "../core/perlim_noise.hpp"
#include "../entities/belt.hpp"
#include <imgui.h>
#include <iomanip>

WorldScene::WorldScene(): noiser{static_cast<uint32_t>(time(nullptr))}
{
    for (int64_t i = 0; i < 4; ++i)
    {
        this->world[4 + i][4].push_back(this->belts.size());
        belts.emplace_back(glm::u64vec2{4 + i, 4}, Belt::Rotation::RIGHT);
    }
    Belt* b     = &this->belts.at(0);
    b->resource = Resource{b->world_index, Resource::ResourceType::COOPER};
    for (int64_t i = 0; i < 4; ++i)
    {
        this->world[4 + 4][4 + i].push_back(this->belts.size());
        belts.emplace_back(glm::u64vec2{4 + 4, 4 + i}, Belt::Rotation::DOWN);
    }
    b           = &this->belts.at(4);
    b->resource = Resource{b->world_index, Resource::ResourceType::IRON};

    for (int64_t i = 0; i < 4; ++i)
    {
        this->world[4 + 4 - i][4 + 4].push_back(this->belts.size());
        belts.emplace_back(glm::u64vec2{4 + 4 - i, 4 + 4}, Belt::Rotation::LEFT);
    }
}

void WorldScene::try_move_resource(Belt& origin)
{
    glm::u64vec2 const target_index = origin.get_output_index();
    if (this->world[target_index.x][target_index.y].size() > 0)
    {
        Belt& target_belt = this->belts[this->world[target_index.x][target_index.y][0]];
        if (!target_belt.resource.has_value())
        {
            target_belt.resource                  = origin.resource;
            target_belt.resource->world_index     = target_index;
            target_belt.resource->moved_this_tick = true;
            origin.resource                       = std::nullopt;
        }
    }
}

void WorldScene::update_belts()
{
    for (Belt& belt: this->belts)
        if (belt.resource.has_value())
            belt.resource->moved_this_tick = false;

    for (Belt& belt: this->belts)
    {
        if (belt.resource.has_value() && !belt.resource->moved_this_tick)
        {
            this->try_move_resource(belt);
        }
    }
}

std::optional<SceneExit> WorldScene::update(double delta, std::vector<InputEvent> input_events,
                                            SceneGroup& scene_group)
{
    (void)delta;
    (void)input_events;
    (void)scene_group;

    this->update_belts_timer += delta;

    if (this->update_belts_timer > 1)
    {
        this->update_belts_timer -= 1;
        this->update_belts();
    }

    return std::nullopt;
}

void WorldScene::render(ScreenRenderer& renderer) const
{
    for (size_t y = 0; y < Constants::WORLD_SIZE; ++y)
    {
        for (size_t x = 0; x < Constants::WORLD_SIZE; ++x)
        {
            glm::vec2 const tile_size = glm::uvec2{Constants::TILE_SIZE, Constants::TILE_SIZE};
            double const noise = (this->noiser.get_fractal_brownian_motion_2d(glm::dvec2(x, y), 8));
            renderer.draw_rectangle(tile_size * glm::vec2{x, y}, tile_size,
                                    glm::u8vec3{55, 0 + noise * 255, 0});
        }
    }

    for (Belt const& belt: this->belts)
    {
        belt.render(renderer);
    }
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
