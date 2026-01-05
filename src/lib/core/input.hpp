#pragma once

// builtin
#include <memory>

// external
#include <glm/vec2.hpp>



enum class InputEventType
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    PAUSE,
    ROTATE_LEFT,
    ROTATE_RIGHT
};

enum class InputEventState
{
    PRESSED,
    HOLD,
};

class MouseInput
{
public:

    enum class MouseButton
    {
        NONE,
        LEFT,
        MIDDLE,
        RIGHT
    };

    glm::u64vec2 screen_position;
    glm::u64vec2 mouse_index;

    MouseButton button = MouseButton::NONE;
};

class InputEvent
{
public:

    InputEventType type;
    InputEventState state;


    InputEvent(InputEventType const type, InputEventState const state): type{type}, state{state} {}
};

class FrameInput
{
public:

    std::vector<InputEvent> key_inputs;
    MouseInput mouse_input;
};
