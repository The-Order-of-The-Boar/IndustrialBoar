#pragma once

// builtin
#include <source_location>
#include <string>
#include <string_view>

// TODO: get the location where the function where called and print then
// TODO: give colors to the messages


class PanicException final : public std::exception
{
private:

    std::string message;

public:

    explicit PanicException(std::string_view message): message(message) {}
    [[nodiscard]] char const* what() const noexcept override
    {
        return this->message.c_str();
    }
};

void enable_panic_throw_mode();
void disable_panic_throw_mode();
[[nodiscard]] bool get_panic_throw_mode();

[[noreturn]] void panic(std::string_view message,
                        std::source_location location = std::source_location::current());
void warn(std::string_view message,
          std::source_location location = std::source_location::current());
void info(std::string_view message,
          std::source_location location = std::source_location::current());


class GameGracefulExit final : public std::exception
{
};
[[noreturn]] void graceful_exit(std::source_location location = std::source_location::current());
