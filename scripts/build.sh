#!/usr/bin/bash

BUILD_DIR="./build/"
BUILD_TYPE="Debug"
GENERATOR="Ninja"
COMPILER="g++-11"

cmake -B ${BUILD_DIR} -G ${GENERATOR} \
    -D CMAKE_CXX_COMPILER=${COMPILER} \
    -D CMAKE_BUILD_TYPE=${BUILD_TYPE}
cmake --build ${BUILD_DIR}
