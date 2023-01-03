cmake_minimum_required(VERSION 3.16)
project(implot VERSION 0.14 LANGUAGES CXX)

add_library(implot STATIC)
add_library(implot::implot ALIAS implot)

target_sources(implot
    PUBLIC
        implot.h
        implot_internal.h
    PRIVATE
        implot.cpp
        implot_demo.cpp
        implot_items.cpp
)

target_include_directories(implot SYSTEM PUBLIC .)
target_link_libraries(implot PRIVATE imgui::imgui)

target_compile_definitions(imgui PUBLIC)

target_compile_features(implot PUBLIC cxx_std_17)
target_compile_options(implot PUBLIC -std=c++17)

set_target_properties(implot 
    PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

# Install static libraries, shared libraries, and executables
install(TARGETS implot
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# Install header files in include directory
install(DIRECTORY "." TYPE INCLUDE FILES_MATCHING PATTERN "*.h")
