#!/usr/bin/bash

BUILD_DIR="./build/"
BUILD_TYPE="Debug"
GENERATOR="Ninja"

cmake -B ${BUILD_DIR} -G ${GENERATOR} -D CMAKE_BUILD_TYPE=${BUILD_TYPE}
cmake --build ${BUILD_DIR}
