#pragma once

// builtin
#include <optional>
#include <source_location>
#include <string_view>

// TODO: capture the source location where the function was called to print it
//       in the message if the assertion fails



void ib_runtime_assert(bool result, std::optional<std::string_view> message = std::nullopt,
                       std::source_location location = std::source_location::current());
void ib_debug_assert(bool result, std::optional<std::string_view> message = std::nullopt,
                     std::source_location location = std::source_location::current());
