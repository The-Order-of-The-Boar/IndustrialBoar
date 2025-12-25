#pragma once

// builtin
#include <array>
#include <cmath>
#include <vector>

// local
#include "../core/constants.hpp"
#include "../core/entity_manager.hpp"
#include "../core/scene.hpp"
#include "../entities/belt.hpp"
#include "../entities/building.hpp"
#include "../entities/mine.hpp"
#include "../entities/tile.hpp"



class WorldScene final : public Scene
{
private:


    std::array<std::array<Tile, Constants::WORLD_SIZE>, Constants::WORLD_SIZE> world;
    std::vector<Belt> belts;
    std::vector<Mine> mines;

public:

    WorldScene();

    Building* get_building(glm::u64vec2 const index)
    {
        auto& ref = this->world[index.x][index.y].building;
        switch (ref.type)
        {
            case BuildingType::BELT:
                return &this->belts.at(ref.id);
            case BuildingType::MINE:
                return &this->mines.at(ref.id);
            case BuildingType::NO_BUILDING:
                return nullptr;

            default:
                return nullptr;
                break;
        }
    }

    void try_move_resource(Building& origin);
    void update_belts();
    void update_mines();
    void tick();
    std::optional<SceneExit> update(double delta, std::vector<InputEvent> input_events,
                                    SceneGroup& scene_group) override;

    void render(ScreenRenderer& renderer) const override;
    void render_hud(ImGuiHandler& renderer) const override;
};
