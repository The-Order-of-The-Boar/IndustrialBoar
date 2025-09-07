// header
#include "world_scene.hpp"
#include <iostream>

#include "../entities/belt.hpp"

WorldScene::WorldScene()
{
    for (int64_t i = 0; i < 4; ++i)
    {
        this->world[4 + i][4].push_back(this->belts.size());
        belts.emplace_back(glm::u64vec2{4 + i, 4}, Belt::Rotation::RIGHT);
    }
    Belt* b = &this->belts.at(0);
    b->resource = Resource{b->world_index, Resource::ResourceType::COOPER};
    for (int64_t i = 0; i < 4; ++i)
    {
        this->world[4 + 4][4 + i].push_back(this->belts.size());
        belts.emplace_back(glm::u64vec2{4 + 4, 4 + i}, Belt::Rotation::DOWN);
    }
    b = &this->belts.at(4);
    b->resource = Resource{b->world_index, Resource::ResourceType::IRON};

    for (int64_t i = 0; i < 4; ++i)
    {
        this->world[4 + 4 - i][4 + 4].push_back(this->belts.size());
        belts.emplace_back(glm::u64vec2{4 + 4 - i, 4 + 4}, Belt::Rotation::LEFT);
    }
}

void WorldScene::try_move_resource(Belt& origin)
{
    const glm::u64vec2 target_index = origin.get_output_index();
    if (this->world[target_index.x][target_index.y].size() > 0)
    {
        Belt& target_belt = this->belts[this->world[target_index.x][target_index.y][0]];
        if (!target_belt.resource.has_value())
        {
            target_belt.resource = origin.resource;
            target_belt.resource->world_index = target_index;
            target_belt.resource->moved_this_tick = true;
            origin.resource = std::nullopt;
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
    renderer.draw_rectangle({0, 0}, {1024, 1024}, glm::u8vec3{0, 255, 255});
    for (Belt const& belt: this->belts)
    {
        belt.render(renderer);
    }
}