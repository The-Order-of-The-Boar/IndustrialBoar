#pragma once

// builtin
#include <string_view>

// TODO: capture the source location where the function was called to print it
//       in the message if the assertion fails



void ib_runtime_assert(bool result, std::string_view const message = "");
void ib_debug_assert(bool result, std::string_view const message = "");
