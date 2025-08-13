
// builtin
#include <chrono>
#include <cmath>

// local
#include "core/game_context.hpp"
#include "core/scene.hpp"
#include "utils/log.hpp"



template <typename Func>
double calc_delta_secs(Func const& func)
{
    auto before = std::chrono::high_resolution_clock::now();
    func();
    auto after = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double>(after - before).count();
}

double floor_decimal_digits(double value, uint64_t digits)
{
    auto factor = std::pow(10, digits);
    return std::floor(value * factor) / factor;
}

[[noreturn]]
void game_main()
{
    GameContext game_context{"Industrial Boar", {1024, 1024}};
    SceneManager scene_manager;

    double delta = 0;
    double frame_print_timer = 0;
    while (true)
    {
        frame_print_timer += delta;
        if (frame_print_timer >= 2)
        {
            frame_print_timer = 0;
            info(fmt::format("Frame interval: {}, FPS: {}\n", floor_decimal_digits(delta, 3),
                             (uint64_t)(1 / delta)));
        }

        auto events = game_context.flush_events();
        scene_manager.update(delta, std::move(events));

        delta = calc_delta_secs(
            [&]()
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
