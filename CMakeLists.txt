cmake_minimum_required(VERSION 3.16)

project(pine VERSION 0.1.1 LANGUAGES CXX)

option(PINE_USE_CONAN "Use Conan for package management." ON)

option(PINE_BUILD_SHARED "Build shared library." ON)
option(PINE_BUILD_WARNINGS "Enable compiler warnings." ON)
option(PINE_BUILD_EDITOR "Build editor." OFF)
option(PINE_BUILD_EXAMPLES "Build examples." OFF)
option(PINE_BUILD_TESTS "Build test." OFF)

include(cmake/ProjectSettings.cmake)
include(cmake/PreventInSourceBuild.cmake)

add_library(pine_options INTERFACE)
target_compile_features(pine_options INTERFACE cxx_std_17)

add_library(pine_warnings INTERFACE)

if(PINE_USE_CONAN)
    if(CONAN_EXPORTED)
        include(conanbuildinfo.cmake)
        conan_basic_setup()
    else()
        include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/conan.cmake")
        conan_cmake_run(CONANFILE "${CMAKE_CURRENT_SOURCE_DIR}/conanfile.py"
            BASIC_SETUP 
            CMAKE_TARGETS 
            BUILD missing)
        list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
    endif()
endif()

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

add_subdirectory(pine)
add_subdirectory(resources)

if(PINE_BUILD_EDITOR)
    add_subdirectory(editor)
endif()

if(PINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(PINE_BUILD_TESTS)
    add_subdirectory(test)
endif()
