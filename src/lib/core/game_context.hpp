#pragma once

// builtin
#include <memory>
#include <string>
#include <vector>

// local
#include "graphics/screen_renderer.hpp"
#include "graphics/sdl_sdlr_screen_renderer.hpp"
#include "input.hpp"
#include "utils/assert.hpp"
#include "utils/log.hpp"

// external
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <glm/vec2.hpp>



class GameContext
{
private:

    SDL_Window* window = nullptr;
    std::unique_ptr<SDLRScreenRenderer> screen_renderer = nullptr;

public:

    GameContext(std::string const& window_name, glm::u64vec2 const initial_window_size)
    {
        // initialize SDL context
        ib_runtime_assert(SDL_Init(SDL_INIT_VIDEO) == 0, "Failed to initialize SDL");
        ib_runtime_assert(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG, "Failed to initialize SDL_image");

        // create window
        this->window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, (int)initial_window_size.x,
                                        (int)initial_window_size.y, SDL_WINDOW_SHOWN);
        ib_runtime_assert(this->window != nullptr, "Failed to create SDL window");

        // create screen renderer
        this->screen_renderer = std::make_unique<SDLRScreenRenderer>(this->window);
    }

    ScreenRenderer& get_screen_renderer()
    {
        return *this->screen_renderer;
    }

    std::vector<InputEvent> flush_events() // NOLINT(*-convert-member-functions-to-static)
    {
        std::vector<InputEvent> input_events;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                graceful_exit();
        }

        return input_events;
    }

    ~GameContext()
    {
        // since the dtor's of the class members are called after this dtor, we
        // need to manually trigger the destruction of any object which might
        // depend on SDL or ImGui runtime to perform a graceful shutdown
        this->screen_renderer.reset();

        SDL_DestroyWindow(this->window);
        IMG_Quit();
        SDL_Quit();
    }
};
