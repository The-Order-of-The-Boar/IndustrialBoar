
# ------------------------------ cmake config --------------------------------
cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TOOLCHAIN_FILE ./vcpkg/scripts/buildsystems/vcpkg.cmake CACHE PATH "vcpkg root path")

# remove default flags
set(CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "debug build flags")
set(CMAKE_CXX_FLAGS_RELEASE "" CACHE STRING "release build flags")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "" CACHE STRING "relwithdebinfo build flags")


# |--------------------------------------------------------------------------|
# |                              project config                              |
# |--------------------------------------------------------------------------|

# ------------------------------ project setup -------------------------------
project(IndustrialBoar LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------ compile flags -------------------------------
function(set_compile_flags target)
    
    target_compile_options(${target} PRIVATE

        --std=c++20
        -pedantic -Wall -Wextra -Werror
        -fdiagnostics-color=always

        $<$<CONFIG:DEBUG,RELWITHDEBINFO>:-g>
    )

    target_link_options(${target} PRIVATE

        $<$<CONFIG:DEBUG,RELWITHDEBINFO>:-fsanitize=address>
        $<$<CONFIG:DEBUG,RELWITHDEBINFO>:-fsanitize=undefined>
    )

endfunction()

# for now we do not support microsoft compilers(and hopefully never will...)
if ((NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") AND (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    message(FATAL_ERROR "unsupported compiler")
endif()

# early fail for unimplemented build modes
# TODO: prevent failures for string case mismatch
if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(FATAL_ERROR "unsupported build type")
endif()


# |--------------------------------------------------------------------------|
# |                              project targets                             |
# |--------------------------------------------------------------------------|

# ------------------------------- game target --------------------------------
add_executable(game src/game/main.cpp)
target_link_libraries(game PRIVATE fmt::fmt)
set_compile_flags(game)


# |--------------------------------------------------------------------------|
# |                               dependencies                               |
# |--------------------------------------------------------------------------|
find_package(fmt CONFIG REQUIRED)