#pragma once

// builtin
#include <array>
#include <cmath>
#include <vector>

// local
#include "../core/constants.hpp"
#include "../core/entity_manager.hpp"
#include "../core/scene.hpp"
#include "../core/world.hpp"
#include "../entities/building.hpp"
#include "../entities/managers/construction_manager.hpp"
#include "../entities/missile.hpp"
#include "../entities/tile.hpp"
#include "../menus/build_menu.hpp"


class WorldScene final : public Scene
{
private:

    World world;
    ConstructionManager construction_manager;
    BuildMenu build_menu;

    Missile missile;

public:

    WorldScene();


    void try_move_resource(Building& origin);
    void update_belts();
    void update_mines();
    void tick();
    std::optional<SceneExit> update(double delta, FrameInput const& frame_input,
                                    SceneGroup& scene_group) override;

    void render(ScreenRenderer& renderer) const override;
    void render_hud(ImGuiHandler& hud_handler) const override;
};
