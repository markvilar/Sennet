#!/usr/bin/bash

SOURCE_DIR="."
BUILD_DIR="./build/"
BUILD_TYPE="Debug"

mkdir -p build
conan install ${SOURCE_DIR} --install-folder ${BUILD_DIR} \
    --settings build_type=${BUILD_TYPE} \
    --profile=default --build missing
