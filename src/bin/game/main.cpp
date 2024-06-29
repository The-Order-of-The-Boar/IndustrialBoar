
// local
#include <graphics/game_context.hpp>

// external
#include <cmath>
#include <cstdlib>
#include <fmt/format.h>


[[noreturn]]
void game_main() {

    GameContext game_context{"Industrial Boar",  {1024, 1024}};

    auto example_texture = game_context.get_screen_renderer().load_texture("assets/textures/godot.png");
    double pos = -100;

    while (true) {
        
        pos += 1;
        pos = fmod(pos, 1024);

        game_context.flush_events();

        auto& renderer = game_context.get_screen_renderer();
        renderer.start_frame();
        renderer.draw_texture(example_texture, {(uint64_t)pos, (uint64_t)pos});
        renderer.present();
    }
}

int main() {

    try {
        game_main();
    }
    catch (GameGracefulExit const&) {
        info("Game exited gracefully");
    }
}
