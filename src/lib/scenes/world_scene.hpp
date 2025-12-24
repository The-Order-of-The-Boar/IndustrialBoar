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
#include "../entities/tile.hpp"



class WorldScene final : public Scene
{
private:


    std::array<std::array<Tile, Constants::WORLD_SIZE>, Constants::WORLD_SIZE> world;
    std::vector<Belt> belts;

    double update_belts_timer = 0;

public:

    WorldScene();

    void try_move_resource(Belt& origin);
    void update_belts();
    std::optional<SceneExit> update(double delta, std::vector<InputEvent> input_events,
                                    SceneGroup& scene_group) override;

    void render(ScreenRenderer& renderer) const override;
    void render_hud(ImGuiHandler& renderer) const override;
};
