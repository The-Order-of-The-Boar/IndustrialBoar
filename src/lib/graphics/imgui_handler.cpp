// header
#include "./imgui_handler.hpp"

// builtin
#include <iostream>

// external
#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

// local
#include "../utils/assert.hpp"



ImGuiHandler::ImGuiHandler(SDL_Window* window, SDL_Renderer* renderer)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io{ImGui::GetIO()};

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;


    bool status = ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ib_runtime_assert(status, "Failed to init ImGui for SDLRenderer2");
    status = ImGui_ImplSDLRenderer2_Init(renderer);
    ib_runtime_assert(status, "Failed to init ImGui SDLRenderer2");
}

ImGuiHandler::~ImGuiHandler()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiHandler::start_frame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void ImGuiHandler::end_frame()
{
    ImGui::Render();
}
