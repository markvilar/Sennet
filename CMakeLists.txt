cmake_minimum_required(VERSION 3.16)

project(
    sennet
    VERSION 0.1
    LANGUAGES CXX)

option(SENNET_BUILD_EDITOR "Build editor." ON)
option(SENNET_BUILD_SHARED "Build shared library." OFF)
option(SENNET_BUILD_EXAMPLE "Build examples." ON)
option(SENNET_BUILD_WARNINGS "Enable compiler warnings." ON)

include(cmake/ProjectSettings.cmake)
include(cmake/PreventInSourceBuild.cmake)

add_library(ProjectOptions INTERFACE)
target_compile_features(ProjectOptions INTERFACE cxx_std_17)

add_library(ProjectWarnings INTERFACE)

# Enable cache system.
include(cmake/Cache.cmake)

# Standard compiler warnings.
include(cmake/CompilerWarning.cmake)
set_project_warnings(ProjectWarnings)

# Sanitizer options if supported by compiler.
include(cmake/Sanitizers.cmake)
enable_sanitizers(ProjectOptions)

# Allow static analysis options.
include(cmake/StaticAnalyzers.cmake)

# Doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# Add library.
add_subdirectory(sennet)

if(SENNET_BUILD_EDITOR)
    add_subdirectory(editor)
endif()

# Add examples.
if(SENNET_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
