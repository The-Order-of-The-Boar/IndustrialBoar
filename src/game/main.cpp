
// local
#include "game_context.hpp"

// external
#include <fmt/format.h>



int main() {

    fmt::print("Hello, World!\n");

    GameContext game_context{"Industrial Boar",  {1024, 1024}};

    auto example_texture = game_context.get_screen_renderer().load_texture("assets/textures/godot.png");

    while (true) {

        auto& renderer = game_context.get_screen_renderer();
        renderer.clear({0, 255, 0});
        renderer.draw_texture(example_texture, {0, 0});
        renderer.present();
    }
}
