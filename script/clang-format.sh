#!/usr/bin/bash

# Target all hpp and cpp files in example
find $PWD/example/ -iname "*.hpp" -o -iname "*.cpp" \
    | xargs clang-format -style=file -i -fallback-style=none

# Target all hpp and cpp files in src
find $PWD/src/ -iname "*.hpp" -o -iname "*.cpp" \
    | xargs clang-format -style=file -i -fallback-style=none

exit 0
