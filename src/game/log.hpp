#pragma once

// builtin
#include <string_view>
#include <iostream>

// TODO: use a separated file for definitions
// TODO: get the location where the function where called and print then
// TODO: give colors to the messages



[[noreturn]]
inline void panic(std::string_view const message) {

    std::cout << "PANIC: " << message << "\n";
    std::abort();
}

inline void warn(std::string_view const message) {

    std::cout << "WARNING: " << message << "\n";
}

inline void info(std::string_view const message) {

    std::cout << "INFO: " << message << "\n";
}



class GameGracefulExit{};

inline void graceful_exit() {

    info("Starting graceful exit...");
    throw GameGracefulExit{};
}
