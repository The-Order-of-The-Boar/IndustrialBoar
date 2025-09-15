#pragma once

// builtin
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

// local
#include "camera.hpp"
#include "graphics/imgui_handler.hpp"
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
#include <imgui_impl_sdl2.h>



class GameContext
{
private:

    SDL_Window* window                                  = nullptr;
    std::unique_ptr<SDLRScreenRenderer> screen_renderer = nullptr;
    std::unique_ptr<ImGuiHandler> hud_renderer          = nullptr;
    Camera camera;

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
        this->screen_renderer = std::make_unique<SDLRScreenRenderer>(this->window, &this->camera);

        this->hud_renderer =
            std::make_unique<ImGuiHandler>(this->window, this->screen_renderer->renderer);
    }

    ScreenRenderer& get_screen_renderer()
    {
        return *this->screen_renderer;
    }

    ImGuiHandler& get_hud_renderer()
    {
        return *this->hud_renderer;
    }

    Camera& get_camera()
    {
        return this->camera;
    }

    std::unordered_set<InputEventType> pressed_keys;

    static InputEventType sdl_key_to_input_event_type(SDL_Keycode const sdl_key)
    {
        switch (sdl_key)
        {
            case SDLK_UP:
            case SDLK_w:
                return InputEventType::UP;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                return InputEventType::DOWN;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                return InputEventType::LEFT;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                return InputEventType::RIGHT;
                break;

            default:
                return InputEventType::NONE;
                break;
        }
    }

    std::vector<InputEvent> flush_events() // NOLINT(*-convert-member-functions-to-static)
    {
        std::vector<InputEvent> input_events;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
            {
                graceful_exit();
                continue;
            }

            SDL_Keycode const key_code = event.key.keysym.sym;
            if (event.type == SDL_KEYDOWN)
            {
                InputEventType event_type = sdl_key_to_input_event_type(key_code);
                this->pressed_keys.insert(event_type);
            }
            else if (event.type == SDL_KEYUP)
            {
                InputEventType event_type = sdl_key_to_input_event_type(key_code);
                auto iter                 = this->pressed_keys.find(event_type);
                if (iter != this->pressed_keys.end())
                    this->pressed_keys.erase(event_type);
            }
        }

        for (auto const pressed_key: this->pressed_keys)
        {
            input_events.push_back(pressed_key);
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
