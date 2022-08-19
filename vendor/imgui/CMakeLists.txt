project(imgui VERSION 1.88 LANGUAGES CXX)

set(PROJECT_WARNINGS "")

add_library(imgui STATIC)
add_library(imgui::imgui ALIAS imgui)

find_package(glad REQUIRED)
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

target_include_directories(imgui PUBLIC . backends)
target_link_libraries(imgui 
    PRIVATE
        glfw::glfw
    )

target_compile_definitions(imgui
    PUBLIC
        IMGUI_IMPL_GLFW_OPENGL3
        IMGUI_IMPL_OPENGL_LOADER_GLAD
    )

target_compile_features(imgui PUBLIC cxx_std_17)
target_compile_options(imgui PUBLIC -std=c++17)
