
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
        $<$<CONFIG:DEBUG,RELWITHDEBINFO>:-fno-omit-frame-pointer>
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

# ------------------------------- library target -----------------------------
add_library(industrialboar STATIC
    src/lib/utils/assert.hpp
    src/lib/utils/assert.cpp
    src/lib/utils/log.hpp
    src/lib/utils/log.cpp
    src/lib/core/entity_manager.hpp
    src/lib/core/entity_manager.cpp
)
target_include_directories(industrialboar INTERFACE src/lib)
target_link_libraries(industrialboar PUBLIC fmt::fmt glm::glm SDL2 SDL2_image)
set_compile_flags(industrialboar)

# ------------------------------- game target --------------------------------
add_executable(game src/bin/game/main.cpp)
target_link_libraries(game PRIVATE industrialboar)
set_compile_flags(game)

# ------------------------------ tests target --------------------------------
add_executable(tests src/bin/tests/example.cpp
        src/bin/tests/entity_manager.cpp)
target_link_libraries(tests PRIVATE industrialboar Catch2::Catch2 Catch2::Catch2WithMain)
set_compile_flags(tests)

# |--------------------------------------------------------------------------|
# |                               dependencies                               |
# |--------------------------------------------------------------------------|
find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)
