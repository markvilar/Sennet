#!/usr/bin/bash

# Target all hpp and cpp files in example
find $PWD/examples/ -iname "*.hpp" -o -iname "*.cpp" \
    | xargs clang-format -style=file -i -fallback-style=none

# Target all hpp and cpp files in src
find $PWD/lib/ -iname "*.hpp" -o -iname "*.cpp" \
    | xargs clang-format -style=file -i -fallback-style=none

exit 0
