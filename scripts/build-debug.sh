#!/bin/bash
mkdir -p build && cd build
conan install .. -s build_type=Debug --profile=default
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
cmake --build .
