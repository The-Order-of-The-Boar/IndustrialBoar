#!/usr/bin/env bash
set -e

# remove folders containing build artifacts to force new builds
rm -rf ./build
rm -rf ./vcpkg
rm -rf ./vcpkg_installed

# install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh -disableMetrics
cd ..
./vcpkg/vcpkg install
