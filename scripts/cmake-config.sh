#!/usr/bin/bash

SOURCE_DIR="."
BUILD_DIR="./build/"
BUILD_TYPE="Debug"
GENERATOR="Ninja"
COMPILER="clang++-12"

if [ ! -d ${BUILD_DIR} ]; then
cmake \
    -S ${SOURCE_DIR} \
    -B ${BUILD_DIR} \
    -G ${GENERATOR} \
    -D CMAKE_CXX_COMPILER=${COMPILER} \
    -D CMAKE_BUILD_TYPE=${BUILD_TYPE}
cmake --build ${BUILD_DIR}
fi
