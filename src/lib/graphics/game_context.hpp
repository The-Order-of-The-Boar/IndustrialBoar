#pragma once

// builtin
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <string>
#include <memory>

// local
#include "../utils/assert.hpp"
#include "../utils/log.hpp"
#include "screen_renderer.hpp"
#include "sdl_sdlr_screen_renderer.hpp"

// external
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>



class GameContext {
private:

    SDL_Window* window = nullptr;
    std::unique_ptr<SDLRScreenRenderer> screen_renderer = nullptr;

public:

    GameContext(std::string const& window_name, glm::u32vec2 const initial_window_size) {

        // initialize SDL context
        ib_runtime_assert(SDL_Init(SDL_INIT_VIDEO) == 0, "Failed to initialize SDL");
        ib_runtime_assert(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG, "Failed to initialize SDL_image");
        
        // create window
        this->window = SDL_CreateWindow(
            window_name.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            initial_window_size.x, initial_window_size.y,
            SDL_WINDOW_SHOWN
        );
        ib_runtime_assert(this->window != nullptr, "Failed to create SDL window");

        // create screen renderer
        this->screen_renderer = std::make_unique<SDLRScreenRenderer>(this->window);
    }

    ScreenRenderer& get_screen_renderer() {

        return *this->screen_renderer;
    }

    void flush_events() {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                graceful_exit();
        }
    }

    ~GameContext() {

        // since the dtor's of the class members are called after this dtor, we
        // need to manually trigger the destruction of any object which might
        // depend on SDL or ImGui runtime to perform an graceful shutdown
        this->screen_renderer.reset();

        SDL_DestroyWindow(this->window);
        IMG_Quit();
        SDL_Quit();
    }
};
