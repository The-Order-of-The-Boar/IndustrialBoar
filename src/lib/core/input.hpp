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

    InputEvent(InputEventType const type): type{type} {}

private:

    std::unique_ptr<InputEventData*> data = nullptr;
};
