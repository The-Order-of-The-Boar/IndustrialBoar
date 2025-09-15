#pragma once

// builtin
#include <unordered_map>
#include <vector>

// local
#include "../graphics/imgui_handler.hpp"
#include "../graphics/screen_renderer.hpp"
#include "../utils/assert.hpp"
#include "camera.hpp"
#include "input.hpp"



enum class SceneName
{
    MAIN_MENU,
    WORLD_SCENE
};

struct SceneExit
{
    SceneName next_scene;
    bool destroy_current;
};

class SceneGroup;

class Scene
{
public:

    virtual ~Scene() = default;

    virtual std::optional<SceneExit> update(double delta, std::vector<InputEvent> input_events,
                                            SceneGroup& scene_group) = 0;
    virtual void render(ScreenRenderer& renderer) const              = 0;
    virtual void render_hud(ImGuiHandler& renderer) const            = 0;
};



class SceneGroup
{
private:

    std::unordered_map<SceneName, std::unique_ptr<Scene>> scenes;

public:

    SceneGroup() = default;

    bool scene_exists(SceneName scene_name) const;
    Scene& get_scene(SceneName scene_name) const;
    void init_scene(SceneName scene_name, std::unique_ptr<Scene> scene);
    void remove_scene(SceneName scene_name);
};



class SceneManager
{
private:

    SceneGroup scene_group;
    SceneName current_scene;

public:

    explicit SceneManager();

public:

    void update(double delta, std::vector<InputEvent> input_events, Camera& camera);
    void render(ScreenRenderer& renderer) const;
    void render_hud(ImGuiHandler& renderer) const;
};
