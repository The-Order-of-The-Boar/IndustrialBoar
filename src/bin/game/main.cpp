
// builtin
#include <cmath>
#include <chrono>

// local
#include "core/game_context.hpp"
#include "core/scene.hpp"



template <typename Func>
double calc_delta_secs(Func const& func)
{
    auto before = std::chrono::high_resolution_clock::now();
    func();
    auto after = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double>(after - before).count();
}

[[noreturn]]
void game_main()
{
    GameContext game_context{"Industrial Boar", {1024, 1024}};
    SceneManager scene_manager;

    double delta = 0;
    while (true)
    {
        fmt::print("delta: {}\n", delta);
        auto events = game_context.flush_events();
        scene_manager.update(delta, std::move(events));

        delta = calc_delta_secs([&]()
        {
            auto& renderer = game_context.get_screen_renderer();
            renderer.clear();
            scene_manager.render(renderer);
            renderer.present();
        });
    }
}

int main()
{
    try
    {
        game_main();
    }
    catch (GameGracefulExit const&)
    {
        info("Game exited gracefully");
    }
}
