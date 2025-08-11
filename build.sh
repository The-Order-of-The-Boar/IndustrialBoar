#!/usr/bin/env bash
set -e


if !([ -e "vcpkg" ]); then
    echo "uninitialized project"
    exit 1
fi

if (($# < 1)) || (($# > 1)); then
    echo "invalid number of parameters, usage: build.sh <build type>"
    exit 1
fi

build_type=$1
build_dir="./build/$(echo "$build_type" | tr '[:upper:]' '[:lower:]')"

mkdir -p $build_dir
cmake -B $build_dir -S . -DCMAKE_BUILD_TYPE=$build_type
cmake --build $build_dir -j`getconf _NPROCESSORS_ONLN`

# move the generated compile_commands.json file to the root build folder, where
# it is usually expected by language servers
mv $build_dir/compile_commands.json ./build/compile_commands.json
