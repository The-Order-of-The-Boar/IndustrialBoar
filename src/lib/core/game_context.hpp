#pragma once

// builtin
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// local
#include "camera.hpp"
#include "graphics/imgui_handler.hpp"
#include "graphics/screen_renderer.hpp"
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

    SDL_Window* window                              = nullptr;
    std::unique_ptr<ScreenRenderer> screen_renderer = nullptr;
    std::unique_ptr<ImGuiHandler> hud_renderer      = nullptr;
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
        this->screen_renderer = std::make_unique<ScreenRenderer>(this->window, &this->camera);

        this->hud_renderer = std::make_unique<ImGuiHandler>(
            this->window, this->screen_renderer->renderer, this->screen_renderer.get());
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

    std::unordered_map<InputEventType, InputEventState> pressed_keys;
    MouseInput mouse_input;

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
            case SDLK_p:
                return InputEventType::PAUSE;
                break;
            case SDLK_q:
                return InputEventType::ROTATE_LEFT;
            case SDLK_e:
                return InputEventType::ROTATE_RIGHT;

            default:
                return InputEventType::NONE;
                break;
        }
    }

    FrameInput flush_events() // NOLINT(*-convert-member-functions-to-static)
    {
        FrameInput frame_input;
        for (auto& key: this->pressed_keys)
        {
            key.second = InputEventState::HOLD;
        }

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
                InputEventType event_type      = sdl_key_to_input_event_type(key_code);
                this->pressed_keys[event_type] = InputEventState::PRESSED;
            }
            else if (event.type == SDL_KEYUP)
            {
                InputEventType event_type = sdl_key_to_input_event_type(key_code);
                auto iter                 = this->pressed_keys.find(event_type);
                if (iter != this->pressed_keys.end())
                    this->pressed_keys.erase(event_type);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                this->mouse_input.button =
                    static_cast<MouseInput::MouseButton>(event.button.button);
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                MouseInput::MouseButton const released_button =
                    static_cast<MouseInput::MouseButton>(event.button.button);

                if (this->mouse_input.button == released_button)
                {
                    this->mouse_input.button = MouseInput::MouseButton::NONE;
                }
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                this->mouse_input.screen_position = glm::u64vec2(event.motion.x, event.motion.y);
            }
        }

        for (auto const& [pressed_key, state]: this->pressed_keys)
        {
            frame_input.key_inputs.emplace_back(pressed_key, state);
        }

        frame_input.mouse_input = this->mouse_input;
        return frame_input;
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
