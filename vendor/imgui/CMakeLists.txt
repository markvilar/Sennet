cmake_minimum_required(VERSION 3.16)
project(imgui VERSION 1.88 LANGUAGES CXX)

add_library(imgui STATIC)
add_library(imgui::imgui ALIAS imgui)

find_package(glfw3 REQUIRED)

target_sources(imgui
    PUBLIC
        backends/imgui_impl_glfw.h
        backends/imgui_impl_opengl3.h
        backends/imgui_impl_opengl3_loader.h
        imconfig.h
        imgui.h
        imgui_internal.h
    PRIVATE
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp
)

target_include_directories(imgui SYSTEM PUBLIC .)
target_link_libraries(imgui PRIVATE glfw::glfw)

target_compile_definitions(imgui
    PUBLIC
        IMGUI_IMPL_GLFW_OPENGL3
        IMGUI_IMPL_OPENGL_LOADER_GLAD
)

target_compile_features(imgui PUBLIC cxx_std_17)
target_compile_options(imgui PUBLIC -std=c++17)

set_target_properties(imgui PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

# Install static libraries, shared libraries, and executables
install(TARGETS imgui
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# Install header files in include directory
install(DIRECTORY "." TYPE INCLUDE FILES_MATCHING PATTERN "*.h")
