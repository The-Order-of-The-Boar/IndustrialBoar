// header
#include "log.hpp"

// external
#include <fmt/format.h>



static bool panic_throw_mode = false;

void enable_panic_throw_mode()
{
    panic_throw_mode = true;
}

void disable_panic_throw_mode()
{
    panic_throw_mode = false;
}

bool get_panic_throw_mode()
{
    return panic_throw_mode;
}


std::string format_source_location(std::source_location location)
{
    return fmt::format("[{}]({}:{})", location.file_name(), location.line(), location.column());
}

void panic(std::string_view const raw_message, std::source_location location)
{
    std::string message =
        fmt::format("{} PANIC: {}", format_source_location(location), raw_message);

    fmt::print("{}\n", message);

    if (panic_throw_mode)
        throw PanicException{message};
    else
        std::abort();
}

void warn(std::string_view const raw_message, std::source_location location)
{
    std::string message = fmt::format("{} WARN: {}", format_source_location(location), raw_message);
    fmt::print("{}\n", message);
}

void info(std::string_view const raw_message, std::source_location location)
{
    std::string message = fmt::format("{} INFO: {}", format_source_location(location), raw_message);
    fmt::print("{}\n", message);
}



void graceful_exit(std::source_location location)
{
    info("Starting graceful exit...", location);
    throw GameGracefulExit{};
}
