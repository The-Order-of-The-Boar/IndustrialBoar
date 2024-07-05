#pragma once

// builtin
#include <cmath>

// local
#include "../core/scene.hpp"



class MainMenu final: public Scene
{
private:

    double pos = -100;
    mutable std::optional<TextureID> example_texture;

public:

    MainMenu() = default;

    std::optional<SceneExit> update(double delta, std::vector<InputEvent> input_events,
                                    SceneGroup& scene_group) override
    {
        (void)input_events;
        (void)scene_group;

        this->pos += (delta * 60 * 5);
        this->pos = fmod(this->pos, 1024);

        return std::nullopt;
    }

    void render(ScreenRenderer& renderer) const override
    {
        if (this->example_texture.has_value() == false)
            this->example_texture.emplace(renderer.load_texture("assets/textures/godot.png"));

        renderer.draw_texture(this->example_texture.value(), {(uint64_t)this->pos, (uint64_t)this->pos});
    }
};
