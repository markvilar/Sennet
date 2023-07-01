cmake_minimum_required(VERSION 3.16)

project(pine VERSION 0.4.0 LANGUAGES CXX)

# -----------------------------------------------------------------------------
# ---- CXX settings -----------------------------------------------------------
# -----------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

# -----------------------------------------------------------------------------
# ---- Project options --------------------------------------------------------
# -----------------------------------------------------------------------------

option(PINE_SANITIZERS_ENABLED "Enable sanitizers." ON)
option(PINE_WARNINGS_ENABLED "Enable compiler warnings." ON)

option(PINE_EDITOR_ENABLED "Build editor." ON)
option(PINE_EXAMPLE_ENABLED "Build examples." ON)
option(PINE_TEST_ENABLED "Build test." ON)

# -----------------------------------------------------------------------------
# ---- Warnings / sanitizers / tools ------------------------------------------
# -----------------------------------------------------------------------------

include(cmake/project_settings.cmake)
include(cmake/prevent_in_source_build.cmake)

# Interface library for options
add_library(options INTERFACE)
add_library(pine::options ALIAS options)
target_compile_features(options INTERFACE cxx_std_20)

# Interface library for warnings
add_library(warnings INTERFACE)
add_library(pine::warnings ALIAS warnings)

# Enable cache system.
include(cmake/cache.cmake)

# Standard compiler warnings.
if (PINE_WARNINGS_ENABLED)
    include(cmake/compiler_warning.cmake)
    set_project_warnings(warnings)
endif()

# Sanitizer options if supported by compiler.
if (PINE_SANITIZERS_ENABLED)
    include(cmake/sanitizers.cmake)
    enable_sanitizers(options)
endif()

# Allow static analysis options.
include(cmake/static_analyzers.cmake)

# Enable doxygen
include(cmake/doxygen.cmake)
enable_doxygen()

# -----------------------------------------------------------------------------
# ---- Source / Vendor / Test -------------------------------------------------
# -----------------------------------------------------------------------------

# Add library, resources, and dependencies built by the project
add_subdirectory(lib)
add_subdirectory(resources)
add_subdirectory(vendor)

# Add examples
if(PINE_EXAMPLE_ENABLED)
    add_subdirectory(examples)
endif()

# Add tests
if(PINE_TEST_ENABLED)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()
