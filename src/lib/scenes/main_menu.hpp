#pragma once

// builtin
#include <cmath>

// local
#include "../core/scene.hpp"



class MainMenu final : public Scene
{
private:

    double pos = -100;
    TextureIDHolder example_texture{"assets/textures/godot.png"};

public:

    MainMenu() = default;

    std::optional<SceneExit> update(double delta, std::vector<InputEvent> input_events,
                                    SceneGroup& scene_group) override
    {
        (void)input_events;
        (void)scene_group;

        this->pos += (delta * 200 * 5);
        this->pos = fmod(this->pos, 1024);

        if (this->pos > 1000)
        {
            SceneExit exit;
            exit.destroy_current = true;
            exit.next_scene      = SceneName::WORLD_SCENE;
            return exit;
        }
        else
        {
            return std::nullopt;
        }
    }

    void render(ScreenRenderer& renderer) const override
    {
        renderer.draw_texture(this->example_texture, {(uint64_t)this->pos, (uint64_t)this->pos});
    }

    void render_hud([[maybe_unused]] ImGuiHandler& renderer) const {}
};
