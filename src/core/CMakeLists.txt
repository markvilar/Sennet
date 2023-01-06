add_library(pine STATIC)
add_library(pine::pine ALIAS pine)

target_sources(pine
    PUBLIC 
        include/pine/core/application.hpp
        include/pine/core/assert.hpp
        include/pine/core/common.hpp
        include/pine/core/input.hpp
        include/pine/core/key_codes.hpp
        include/pine/core/layer.hpp
        include/pine/core/log.hpp
        include/pine/core/mouse_codes.hpp
        include/pine/core/platform.hpp
        include/pine/core/timestep.hpp
        include/pine/core/window.hpp
        include/pine/debug/instrumentor.hpp
        include/pine/events/application_event.hpp
        include/pine/events/event.hpp
        include/pine/events/key_event.hpp
        include/pine/events/mouse_event.hpp
        include/pine/gui/common.hpp
        include/pine/gui/graphical_interface.hpp
        include/pine/network/client.hpp
        include/pine/network/connection.hpp
        include/pine/network/server.hpp
        include/pine/network/types.hpp
        include/pine/platform/opengl/buffer.hpp
        include/pine/platform/opengl/common.hpp
        include/pine/platform/opengl/framebuffer.hpp
        include/pine/platform/opengl/context.hpp
        include/pine/platform/opengl/renderer_api.hpp
        include/pine/platform/opengl/shader.hpp
        include/pine/platform/opengl/texture.hpp
        include/pine/platform/windows/window.hpp
        include/pine/platform/linux/input.hpp
        include/pine/platform/linux/window.hpp
        include/pine/renderer/buffer.hpp
        include/pine/renderer/camera.hpp
        include/pine/renderer/common.hpp
        include/pine/renderer/framebuffer.hpp
        include/pine/renderer/graphics_context.hpp
        include/pine/renderer/image.hpp
        include/pine/renderer/quad_renderer.hpp
        include/pine/renderer/render_command.hpp
        include/pine/renderer/renderer.hpp
        include/pine/renderer/renderer_api.hpp
        include/pine/renderer/shader.hpp
        include/pine/renderer/texture.hpp
        include/pine/utils/filesystem.hpp
        include/pine/utils/math.hpp
        include/pine/utils/locked_queue.hpp
    PRIVATE 
        src/core/application.cpp
        src/core/input.cpp
        src/core/layer.cpp
        src/core/log.cpp
        src/gui/common.cpp
        src/gui/graphical_interface.cpp
        src/network/client.cpp
        src/network/connection.cpp
        src/network/server.cpp
        src/platform/opengl/buffer.cpp
        src/platform/opengl/framebuffer.cpp
        src/platform/opengl/context.cpp
        src/platform/opengl/renderer_api.cpp
        src/platform/opengl/shader.cpp
        src/platform/opengl/texture.cpp
        src/platform/windows/input.cpp
        src/platform/windows/window.cpp
        src/platform/linux/input.cpp
        src/platform/linux/window.cpp
        src/renderer/buffer.cpp
        src/renderer/camera.cpp
        src/renderer/framebuffer.cpp
        src/renderer/graphics_context.cpp
        src/renderer/image.cpp
        src/renderer/quad_renderer.cpp
        src/renderer/render_command.cpp
        src/renderer/renderer.cpp
        src/renderer/renderer_api.cpp
        src/renderer/shader.cpp
        src/renderer/texture.cpp
        src/utils/filesystem.cpp
)

target_precompile_headers(pine PRIVATE include/pine/pch.hpp)

find_package(asio REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
find_package(stb REQUIRED)

target_include_directories(pine PUBLIC include PRIVATE src)

target_link_libraries(pine
    PUBLIC 
        asio::asio
        glm::glm
        imgui::imgui
        spdlog::spdlog
    PRIVATE 
        glad::glad 
        glfw::glfw 
        stb::stb
        $<$<BOOL:${PINE_SANITIZERS_ENABLE}>:pine::options>
        $<$<BOOL:${PINE_WARNINGS_ENABLE}>:pine::warnings>
)

if(MSVC)
    target_compile_definitions(pine PUBLIC PINE_PLATFORM_WINDOWS)
elseif(UNIX)
    target_compile_definitions(pine PUBLIC PINE_PLATFORM_LINUX)
endif()

target_compile_definitions(pine
    PUBLIC 
        GLFW_INCLUDE_NONE
        $<$<CONFIG:Debug>:PINE_DEBUG>
        $<$<CONFIG:Release>:PINE_RELEASE>
    PRIVATE
)

target_compile_features(pine PUBLIC cxx_std_17)
target_compile_options(pine PUBLIC -std=c++17)

set_target_properties(pine PROPERTIES 
    CXX_STANDARD 17
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)

# Install static libraries, shared libraries, and executables
install(TARGETS pine
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# Install header files in include directory
install(DIRECTORY "include/pine" TYPE INCLUDE FILES_MATCHING PATTERN "*.hpp")
