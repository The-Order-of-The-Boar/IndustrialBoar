#pragma once

// builtin
#include <memory>



enum class InputEventType
{
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

private:

    std::unique_ptr<InputEventData*> data = nullptr;
};
