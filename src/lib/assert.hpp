#pragma once

// builtin

// local
#include "log.hpp"



inline void ib_runtime_assert(bool result, std::string_view const message = "") {
    
    if (!result)
        panic(message);
}

inline void ib_debug_assert(bool result, std::string_view const message = "") {

#ifndef NDEBUG
    ib_runtime_assert(result, message);
#endif
}
