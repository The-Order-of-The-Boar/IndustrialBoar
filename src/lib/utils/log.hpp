#pragma once

// builtin
#include <string_view>

// TODO: get the location where the function where called and print then
// TODO: give colors to the messages



[[noreturn]]
void panic(std::string_view message);
void warn(std::string_view message);
void info(std::string_view message);


class GameGracefulExit{};

[[noreturn]]
void graceful_exit();
