#!/bin/bash

BUILD_DIR="./build/"
BUILD_TYPE="Debug"
GENERATOR="Ninja"

mkdir -p build
conan install . --install-folder ${BUILD_DIR} \
    --settings build_type=${BUILD_TYPE} \
    --profile=default --build missing
cmake -B ${BUILD_DIR} -G ${GENERATOR} -D CMAKE_BUILD_TYPE=${BUILD_TYPE}
cmake --build ${BUILD_DIR}
