cmake_minimum_required(VERSION 3.16)

# -----------------------------------------------------------------------------
# ---- Project ----------------------------------------------------------------
# -----------------------------------------------------------------------------

project(
    Sennet
    VERSION 0.1
    LANGUAGES CXX)

include(CMake/ProjectSettings.cmake)
include(CMake/PreventInSourceBuild.cmake)

# -----------------------------------------------------------------------------
# ---- Project options and warnings -------------------------------------------
# -----------------------------------------------------------------------------

add_library(ProjectOptions INTERFACE)
target_compile_features(ProjectOptions INTERFACE cxx_std_20)

add_library(ProjectWarnings INTERFACE)

# -----------------------------------------------------------------------------
# ---- Build options ----------------------------------------------------------
# -----------------------------------------------------------------------------

option(SENNET_USE_VENDOR_IMGUI "Use imgui from vendor." OFF)
option(SENNET_BUILD_SHARED "Build shared library." OFF)
option(SENNET_BUILD_EXAMPLE "Build examples." ON)
option(SENNET_BUILD_EDITOR "Build editor." OFF)
option(SENNET_BUILD_WARNINGS "Enable compiler warnings." ON)

# -----------------------------------------------------------------------------
# ---- Compiler configuration -------------------------------------------------
# -----------------------------------------------------------------------------

# Enable cache system.
include(CMake/Cache.cmake)

# Standard compiler warnings.
include(CMake/CompilerWarning.cmake)
set_project_warnings(ProjectWarnings)

# Sanitizer options if supported by compiler.
include(CMake/Sanitizers.cmake)
enable_sanitizers(ProjectOptions)

# Allow static analysis options.
include(CMake/StaticAnalyzers.cmake)

# -----------------------------------------------------------------------------
# ---- Tools ------------------------------------------------------------------
# -----------------------------------------------------------------------------

# Doxygen
include(CMake/Doxygen.cmake)
enable_doxygen()

# Git
include(CMake/GitSetup.cmake)

# Conan
include(CMake/Conan.cmake)
run_conan()

# -----------------------------------------------------------------------------
# ---- Source -----------------------------------------------------------------
# -----------------------------------------------------------------------------

add_subdirectory(Sennet)

if(SENNET_BUILD_EDITOR)
    add_subdirectory(Editor)
endif()
