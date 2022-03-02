#!/usr/bin/bash

SOURCE_DIR="${PWD}"
BUILD_DIR="${SOURCE_DIR}/build/"
CONAN_DIR="${SOURCE_DIR}/packages/"

GENERATOR="Ninja"
BUILD_TYPE="Release"
CXX_COMPILER="clang++-12"

mkdir -p ${BUILD_DIR}
mkdir -p ${CONAN_DIR}

export CMAKE_GENERATOR="${GENERATOR}"
export CMAKE_BUILD_TYPE="${BUILD_TYPE}"
export CMAKE_CXX_COMPILER="${CXX_COMPILER}"

conan install "${SOURCE_DIR}" --install-folder "${CONAN_DIR}" \
    --settings build_type="${BUILD_TYPE}" --build missing

cmake -S "${SOURCE_DIR}" -B "${BUILD_DIR}" -D CMAKE_MODULE_PATH="${CONAN_DIR}" \
    -D PINE_BUILD_EDITOR=ON -D PINE_BUILD_EXAMPLES=ON
cmake --build ${BUILD_DIR}
