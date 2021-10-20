#!/usr/bin/bash

find . -type f -name "CMakeLists.txt" -o -name "*.cmake" \
    | xargs cmake-format -c .cmake-format.yaml -i 

exit 0
