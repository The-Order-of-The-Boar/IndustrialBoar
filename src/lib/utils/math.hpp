#pragma once

// local
#include "assert.hpp"

namespace Math
{
    double lerp(double const t, double const start, double const end)
    {
        ib_debug_assert(t >= 0 && t <= 1.0, "Non-normalized t argument");
        return start + t * (end - start);
    }

    double fade(double const t)
    {
        return ((6 * t - 15) * t + 10) * t * t * t;
    }
} // namespace Math
