// header
#include "assert.hpp"

// local
#include "log.hpp"



void ib_runtime_assert(bool result, std::optional<std::string_view> const message, std::source_location location) {

    if (!result) {

        std::string_view panic_message = "no message provided";
        if (message.has_value())
            panic_message = *message;

        panic(panic_message, location);
    }
}

void ib_debug_assert(bool result, std::optional<std::string_view> const message, std::source_location location) {

    #ifndef NDEBUG
        ib_runtime_assert(result, message, location);
    #endif
}
