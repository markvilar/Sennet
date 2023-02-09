cmake_minimum_required(VERSION 3.16)

project(pine VERSION 0.3.0 LANGUAGES CXX)

option(PINE_CONAN_ENABLE "Use Conan for package management." ON)

# Build options
option(PINE_SANITIZERS_ENABLE "Enable sanitizers." ON)
option(PINE_WARNINGS_ENABLE "Enable compiler warnings." ON)

# Build targets
option(PINE_EDITOR_ENABLE "Build editor." ON)
option(PINE_EXAMPLE_ENABLE "Build examples." ON)
option(PINE_TEST_ENABLE "Build test." ON)

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
if(PINE_CONAN_ENABLE)
    set(PINE_CONAN_PACKAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/conan_packages")
    if(CONAN_EXPORTED)
        include("conanbuildinfo.cmake")
        conan_basic_setup()
    else()
        include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/conan.cmake")
        conan_cmake_run(CONANFILE "${CMAKE_CURRENT_SOURCE_DIR}/conanfile.py"
            BASIC_SETUP 
            CMAKE_TARGETS 
            REMOTE conancenter
            BUILD missing
            INSTALL_FOLDER "${PINE_CONAN_PACKAGE_DIR}"
            OUTPUT_FOLDER "${PINE_CONAN_PACKAGE_DIR}"
        )
        # Add build directory to module path to find Conans' cmake config files.
        list(APPEND CMAKE_MODULE_PATH "${PINE_CONAN_PACKAGE_DIR}")
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

# Add library, resources, and dependencies built by the project
add_subdirectory(src)
add_subdirectory(resources)
add_subdirectory(vendor)

# Add examples
if(PINE_EXAMPLE_ENABLE)
    add_subdirectory(example)
endif()

# Add tests
if(PINE_TEST_ENABLE)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()
