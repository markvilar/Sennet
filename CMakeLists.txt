cmake_minimum_required(VERSION 3.16)

project(pine VERSION 0.3.0 LANGUAGES CXX)

option(PINE_USE_CONAN "Use Conan for package management." ON)

# Build options
option(PINE_BUILD_SHARED "Build shared library." ON)
option(PINE_BUILD_WARNINGS "Enable compiler warnings." ON)
option(PINE_BUILD_EDITOR "Build editor." ON)
option(PINE_BUILD_EXAMPLES "Build examples." OFF)
option(PINE_BUILD_TESTS "Build test." OFF)

include(cmake/project_settings.cmake)
include(cmake/prevent_in_source_build.cmake)

# Interface library for options
add_library(options INTERFACE)
add_library(pine::options ALIAS options)
target_compile_features(options INTERFACE cxx_std_17)

# Interface library for warnings
add_library(warnings INTERFACE)
add_library(pine::warnings ALIAS warnings)

# Use conan for package management
if(PINE_USE_CONAN)
    if(CONAN_EXPORTED)
        include(conanbuildinfo.cmake)
        conan_basic_setup()
    else()
        include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/conan.cmake")
        conan_cmake_run(CONANFILE "${CMAKE_CURRENT_SOURCE_DIR}/conanfile.py"
            BASIC_SETUP 
            CMAKE_TARGETS 
            BUILD missing
        )
        # Add build directory to module path to find Conans' cmake config files.
        list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
    endif()
endif()

# Enable cache system.
include(cmake/cache.cmake)

# Standard compiler warnings.
include(cmake/compiler_warning.cmake)
set_project_warnings(warnings)

# Sanitizer options if supported by compiler.
include(cmake/sanitizers.cmake)
enable_sanitizers(options)

# Allow static analysis options.
include(cmake/static_analyzers.cmake)

# Enable doxygen
include(cmake/doxygen.cmake)
enable_doxygen()

# Add library core, resources, and dependencies
add_subdirectory(src)
add_subdirectory(resources)
add_subdirectory(vendor)

# Add examples
if(PINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Add tests
if(PINE_BUILD_TESTS)
    add_subdirectory(tests)
endif()
