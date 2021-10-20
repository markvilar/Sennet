option(SENNET_BUILD_EDITOR "Build editor." ON)
option(SENNET_BUILD_SHARED "Build shared library." OFF)
option(SENNET_BUILD_EXAMPLE "Build examples." ON)
option(SENNET_BUILD_WARNINGS "Enable compiler warnings." ON)

add_subdirectory(Sennet)

if(SENNET_BUILD_EDITOR)
    add_subdirectory(Editor)
endif()
