// header
#include "log.hpp"

// builtin
#include <iostream>



void panic(std::string_view const message) {

  std::cout << "PANIC: " << message << "\n";
  std::abort();
}

void warn(std::string_view const message) {

  std::cout << "WARNING: " << message << "\n";
}

void info(std::string_view const message) {

  std::cout << "INFO: " << message << "\n";
}



void graceful_exit() {

  info("Starting graceful exit...");
  throw GameGracefulExit{};
}
