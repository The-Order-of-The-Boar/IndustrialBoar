#include "screen_renderer.hpp"

#include <iostream>

ScreenRenderer::ScreenRenderer(SDL_Window* window, Camera const* const camera): camera{camera}
{
    this->renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    ib_runtime_assert(this->renderer != nullptr, "Failed to create SDL renderer");
}

ScreenRenderer::~ScreenRenderer()
{
    // since the dtor of the class members are called after this dtor, we
    // need to manually trigger the destruction of any object which might
    // depend on the renderer to perform a graceful shutdown.
    this->textures.clear();

    SDL_DestroyRenderer(this->renderer);
}

SDLRendererTexture ScreenRenderer::load_sdl_texture(std::string_view const path)
{
    SDL_Texture* const texture = IMG_LoadTexture(this->renderer, path.data());
    ib_runtime_assert(texture != nullptr, "Failed to create SDL texture");

    return SDLRendererTexture{texture};
}

TextureID ScreenRenderer::load_texture(std::string_view const path)
{
    SDLRendererTexture texture = this->load_sdl_texture(path);
    this->textures.insert({this->next_texture_id, std::move(texture)});
    return TextureID{this->next_texture_id++};
}

SDLRendererTexture& ScreenRenderer::get_texture(TextureIDHolder& texture_holder)
{
    if (!texture_holder.texture_id.has_value())
        texture_holder.texture_id.emplace(this->load_texture(texture_holder.path));

    return this->textures.at(texture_holder.texture_id.value().id);
}

void ScreenRenderer::clear(glm::u8vec3 const clear_color)
{
    SDL_SetRenderDrawColor(this->renderer, clear_color.r, clear_color.g, clear_color.b, 255);
    SDL_RenderClear(this->renderer);
}

void ScreenRenderer::draw_texture(TextureIDHolder const& texture, glm::vec2 const world_position,
                                  std::optional<glm::u64vec2> custom_draw_size,
                                  std::optional<glm::u8vec3> modulate)
{
    SDLRendererTexture& sdl_texture = this->get_texture(texture);
    glm::u64vec2 const draw_size    = custom_draw_size.value_or(sdl_texture.get_size());

    glm::vec2 const screen_position = this->world_to_screen_position(world_position);
    if (!this->is_visible(screen_position, draw_size))
        return;
    SDL_Rect const dest_rect = {(int)screen_position.x, (int)screen_position.y, (int)draw_size.x,
                                (int)draw_size.y};

    if (modulate.has_value())
        SDL_SetTextureColorMod(sdl_texture.texture, modulate->r, modulate->g, modulate->b);
    SDL_RenderCopy(this->renderer, sdl_texture.texture, nullptr, &dest_rect);
}

void ScreenRenderer::draw_rectangle(glm::vec2 const world_position, glm::u64vec2 const size,
                                    glm::u8vec3 const color)
{
    SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, 255);
    glm::vec2 const screen_position = this->world_to_screen_position(world_position);
    if (!this->is_visible(screen_position, size))
    {
        return;
    }

    SDL_Rect const rect = {(int)screen_position.x, (int)screen_position.y, (int)size.x,
                           (int)size.y};
    SDL_RenderFillRect(this->renderer, &rect);
}

void ScreenRenderer::draw_line(glm::u64vec2 const start, glm::u64vec2 const end,
                               glm::u8vec3 const color)
{
    // TODO: culling
    SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, 255);
    SDL_RenderDrawLine(this->renderer, (int)start.x, (int)start.y, (int)end.x, (int)end.y);
}

void ScreenRenderer::start_frame()
{
    this->clear({0, 0, 0});
}

void ScreenRenderer::present()
{
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), this->renderer);
    SDL_RenderPresent(this->renderer);
}

SDLRendererTexture& ScreenRenderer::get_texture(TextureIDHolder const& texture)
{
    if (!texture.texture_id.has_value())
        texture.texture_id.emplace(this->load_texture(texture.path));

    return this->textures.at(texture.texture_id.value().id);
}


glm::vec2 ScreenRenderer::world_to_screen_position(glm::vec2 const world_position) const
{
    return world_position + this->camera->get_position();
}

bool ScreenRenderer::is_visible(glm::vec2 const pos, glm::vec2 const size) const
{
    if (pos.x > Constants::SCREEN_SIZE.x || pos.y > Constants::SCREEN_SIZE.y)
        return false;

    if (pos.x + size.x < 0 || pos.y + size.y < 0)
        return false;

    return true;
}
