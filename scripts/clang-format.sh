#!/usr/bin/bash

find ./editor -iname *.hpp -o -iname *.cpp \
    | xargs clang-format -style=file -i -fallback-style=none

find ./examples -iname *.hpp -o -iname *.cpp \
    | xargs clang-format -style=file -i -fallback-style=none

find ./pine -iname *.hpp -o -iname *.cpp \
    | xargs clang-format -style=file -i -fallback-style=none

exit 0
