#!/bin/bash
mkdir -p build && cd build
conan install .. -s build_type=Release --profile=default
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build .
