#pragma once

// builtin
#include <ostream>
#include <vector>

// external
#include <glm/vec2.hpp>


template <class ElementT>
inline std::ostream& operator<<(std::ostream& ostream, std::vector<ElementT> const& vector)
{
    ostream << "[ ";
    for (size_t i = 0; i < vector.size() - 1; i++)
    {
        ostream << vector[i] << ", ";
    }
    ostream << vector[vector.size() - 1] << " ]\n";

    return ostream;
}

inline std::ostream& operator<<(std::ostream& ostream, glm::vec2 const vec2)
{
    ostream << "(" << vec2.x << ", " << vec2.y << ")";
    return ostream;
}
