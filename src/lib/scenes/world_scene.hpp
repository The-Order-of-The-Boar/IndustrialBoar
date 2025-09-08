#pragma once

// builtin
#include <array>
#include <cmath>
#include <vector>

// local
#include "../core/constants.hpp"
#include "../core/entity_manager.hpp"
#include "../core/perlim_noise.hpp"
#include "../core/scene.hpp"
#include "../entities/belt.hpp"
#include "../graphics/graphic_entity.hpp"



class WorldScene final : public Scene
{
private:


    std::array<std::array<std::vector<size_t>, Constants::WORLD_SIZE>, Constants::WORLD_SIZE> world;
    std::vector<Belt> belts;

    PerlimNoise noiser;

    double update_belts_timer = 0;

public:

    WorldScene();

    void try_move_resource(Belt& origin);
    void update_belts();
    std::optional<SceneExit> update(double delta, std::vector<InputEvent> input_events,
                                    SceneGroup& scene_group) override;

    void render(ScreenRenderer& renderer) const override;
};
