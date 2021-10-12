cmake_minimum_required(VERSION 3.16)

project(
    sennet
    VERSION 0.1
    LANGUAGES CXX)

include(cmake/ProjectSettings.cmake)
include(cmake/PreventInSourceBuild.cmake)

# Add build directory to module and prefix path.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

add_library(ProjectOptions INTERFACE)
target_compile_features(ProjectOptions INTERFACE cxx_std_20)

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

# Conan
#include(cmake/Conan.cmake)
#run_conan()

# Add library.
add_subdirectory(src)

# Add examples.
add_subdirectory(examples)
