#pragma once

// builtin
#include <cstddef>

enum BuildingType
{
    NO_BUILDING = 0,
    BELT,
    MINE,
};


class BuildingRef
{
public:

    BuildingType type = BuildingType::NO_BUILDING;
    size_t id;
};
