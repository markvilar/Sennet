#!/usr/bin/bash

SOURCE_DIR="${PWD}"
BUILD_DIR="${SOURCE_DIR}/build/"
CONAN_DIR="${SOURCE_DIR}/conan-packages/"

GENERATOR="Ninja"
BUILD_TYPE="Debug"
CXX_COMPILER="clang++-12"

if [ ! -d ${BUILD_DIR} ]; then
    mkdir -p ${BUILD_DIR}

    export CMAKE_GENERATOR="${GENERATOR}"
    export CMAKE_BUILD_TYPE="${BUILD_TYPE}"
    export CMAKE_CXX_COMPILER="${CXX_COMPILER}"

    conan install "${SOURCE_DIR}" \
        --install-folder "${CONAN_DIR}" \
        --settings build_type="${BUILD_TYPE}" \
        --profile=default \
        --build missing

    cmake -S "${SOURCE_DIR}" -B "${BUILD_DIR}" \
        -D CMAKE_MODULE_PATH="${CONAN_DIR}"
fi

cmake --build ${BUILD_DIR}
