#pragma once

// builtin
#include <memory>



enum class InputEventType
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    PAUSE,
};

enum class InputEventState
{
    PRESSED,
    HOLD,
};

class InputEventData
{
public:

    virtual ~InputEventData() = default;
};

class InputEvent
{
public:

    InputEventType type;
    InputEventState state;


    InputEvent(InputEventType const type, InputEventState const state): type{type}, state{state} {}

private:

    std::unique_ptr<InputEventData*> data = nullptr;
};
