#!/usr/bin/env bash
set -e

GAME_TARGET="game"

if (($# < 1)) || (($# > 1)); then
    echo "invalid number of parameters, usage: run.sh <build type>"
    exit 1
fi

build_type=$1
build_dir="./build/$(echo "$build_type" | tr '[:upper:]' '[:lower:]')"

# run the build script first, to ensure that the project is built with the
# latest changes and to ensure that the project is built with the correct
./build.sh $build_type

# setup AddressSanitizer suppressions
export LSAN_OPTIONS='suppressions=surpress_leaks.txt,report_objects=1'

# run the target
echo ""
echo "Running the target..."
echo "---------------------"
$build_dir/$GAME_TARGET
