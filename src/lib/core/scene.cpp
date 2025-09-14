// header
#include "scene.hpp"

// local
#include "../scenes/main_menu.hpp"
#include "../scenes/world_scene.hpp"



bool SceneGroup::scene_exists(SceneName scene_name) const
{
    return this->scenes.contains(scene_name);
}

Scene& SceneGroup::get_scene(SceneName scene_name) const
{
    ib_runtime_assert(this->scenes.contains(scene_name), "The requested scene does not exist");
    return *this->scenes.at(scene_name);
}

void SceneGroup::init_scene(SceneName scene_name, std::unique_ptr<Scene> scene)
{
    ib_runtime_assert(!this->scenes.contains(scene_name), "The scene already exists");
    this->scenes.emplace(scene_name, std::move(scene));
}

void SceneGroup::remove_scene(SceneName scene_name)
{
    ib_runtime_assert(this->scenes.contains(scene_name), "The scene does not exist");
    this->scenes.erase(scene_name);
}



SceneManager::SceneManager()
{
    this->scene_group.init_scene(SceneName::MAIN_MENU, std::make_unique<MainMenu>());
    this->scene_group.init_scene(SceneName::WORLD_SCENE, std::make_unique<WorldScene>());
    this->current_scene = SceneName::MAIN_MENU;
}

void SceneManager::update(double delta, std::vector<InputEvent> input_events)
{
    auto& scene     = this->scene_group.get_scene(this->current_scene);
    auto scene_exit = scene.update(delta, std::move(input_events), this->scene_group);

    if (scene_exit.has_value())
    {
        if (scene_exit->destroy_current)
            this->scene_group.remove_scene(this->current_scene);

        ib_runtime_assert(this->scene_group.scene_exists(scene_exit->next_scene),
                          "The next scene does not exist");
        this->current_scene = scene_exit->next_scene;
    }
}

void SceneManager::render(ScreenRenderer& renderer) const
{
    auto& scene = this->scene_group.get_scene(this->current_scene);
    scene.render(renderer);
}

void SceneManager::render_hud(ImGuiHandler& renderer) const
{
    auto& scene = this->scene_group.get_scene(this->current_scene);
    scene.render_hud(renderer);
}
