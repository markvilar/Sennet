cmake_minimum_required(VERSION 3.16)

project(
    pine
    VERSION 0.1
    LANGUAGES CXX)

option(PINE_BUILD_SHARED "Build shared library." ON)
option(PINE_BUILD_EDITOR "Build editor." OFF)
option(PINE_BUILD_EXAMPLES "Build examples." OFF)
option(PINE_BUILD_WARNINGS "Enable compiler warnings." ON)

include(cmake/ProjectSettings.cmake)
include(cmake/PreventInSourceBuild.cmake)

add_library(pine_options INTERFACE)
target_compile_features(pine_options INTERFACE cxx_std_17)

add_library(pine_warnings INTERFACE)

# Enable cache system.
include(cmake/Cache.cmake)

# Standard compiler warnings.
include(cmake/CompilerWarning.cmake)
set_project_warnings(pine_warnings)

# Sanitizer options if supported by compiler.
include(cmake/Sanitizers.cmake)
enable_sanitizers(pine_options)

# Allow static analysis options.
include(cmake/StaticAnalyzers.cmake)

# Doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# Add library.
add_subdirectory(pine)

if(PINE_BUILD_EDITOR)
    add_subdirectory(editor)
endif()

# Add examples.
if(PINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
