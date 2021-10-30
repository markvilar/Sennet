#!/usr/bin/bash

BUILD_DIR="./build/"
BUILD_TYPE="Debug"

mkdir -p build
conan install . --install-folder ${BUILD_DIR} \
    --settings build_type=${BUILD_TYPE} \
    --profile=default --build missing
