add_library(pine STATIC)
add_library(pine::pine ALIAS pine)

target_sources(pine
    PUBLIC 
        # Collection header
        src/pine/pine.hpp

        # Defines and macros
        src/pine/defines/assert.hpp
        src/pine/defines/platform.hpp

    PRIVATE
        # Core
        src/pine/core/engine.cpp
        src/pine/core/engine.hpp
        src/pine/core/event.hpp
        src/pine/core/input.cpp
        src/pine/core/input.hpp
        src/pine/core/log.cpp
        src/pine/core/log.hpp
        src/pine/core/timestep.hpp
        src/pine/core/window.cpp
        src/pine/core/window.hpp

        # Gui
        src/pine/gui/flags.hpp
        src/pine/gui/manager.cpp
        src/pine/gui/manager.hpp
        src/pine/gui/style.cpp
        src/pine/gui/style.hpp
        src/pine/gui/widgets.hpp
        src/pine/gui/window.cpp
        src/pine/gui/window.hpp

        # Network
        src/pine/network/client.cpp
        src/pine/network/client.hpp
        src/pine/network/connection.cpp
        src/pine/network/connection.hpp
        src/pine/network/server.cpp
        src/pine/network/server.hpp
        src/pine/network/types.hpp

        # Platform - OpenGL
        src/pine/platform/opengl/buffer.cpp
        src/pine/platform/opengl/buffer.hpp
        src/pine/platform/opengl/context.cpp
        src/pine/platform/opengl/context.hpp
        src/pine/platform/opengl/framebuffer.cpp
        src/pine/platform/opengl/framebuffer.hpp
        src/pine/platform/opengl/renderer_api.cpp
        src/pine/platform/opengl/renderer_api.hpp
        src/pine/platform/opengl/shader.cpp
        src/pine/platform/opengl/shader.hpp
        src/pine/platform/opengl/texture.cpp
        src/pine/platform/opengl/texture.hpp
        src/pine/platform/opengl/utilities.cpp
        src/pine/platform/opengl/utilities.hpp

        # Platform - Linux
        src/pine/platform/linux/input.cpp
        src/pine/platform/linux/input.hpp
        src/pine/platform/linux/window.cpp
        src/pine/platform/linux/window.hpp
        
        # Platform - Windows
        src/pine/platform/windows/input.cpp
        src/pine/platform/windows/input.hpp
        src/pine/platform/windows/window.cpp
        src/pine/platform/windows/window.hpp

        # Renderer
        src/pine/renderer/buffer.cpp
        src/pine/renderer/buffer.hpp
        src/pine/renderer/camera.cpp
        src/pine/renderer/camera.hpp
        src/pine/renderer/framebuffer.cpp
        src/pine/renderer/framebuffer.hpp
        src/pine/renderer/graphics_context.cpp
        src/pine/renderer/graphics_context.hpp
        src/pine/renderer/image.cpp
        src/pine/renderer/image.hpp
        src/pine/renderer/quad_renderer.cpp
        src/pine/renderer/quad_renderer.hpp
        src/pine/renderer/render_command.cpp
        src/pine/renderer/render_command.hpp
        src/pine/renderer/renderer.cpp
        src/pine/renderer/renderer.hpp
        src/pine/renderer/renderer_api.cpp
        src/pine/renderer/renderer_api.hpp
        src/pine/renderer/shader.cpp
        src/pine/renderer/shader.hpp
        src/pine/renderer/texture.cpp
        src/pine/renderer/texture.hpp
        src/pine/renderer/types.cpp
        src/pine/renderer/types.hpp

        # Utils
        src/pine/utils/argparse.cpp
        src/pine/utils/argparse.hpp
        src/pine/utils/fonts.hpp
        src/pine/utils/filesystem.cpp
        src/pine/utils/filesystem.hpp
        src/pine/utils/locked_queue.hpp
        src/pine/utils/math.hpp
        src/pine/utils/uuid.cpp
        src/pine/utils/uuid.hpp

        # NOTE: Experimental
        src/pine/graphics/api.cpp
        src/pine/graphics/api.hpp
        src/pine/platform/glfw/api.cpp
        src/pine/platform/glfw/api.hpp
        src/pine/platform/glfw/window.cpp
        src/pine/platform/glfw/window.hpp
)

target_precompile_headers(pine PRIVATE src/pine/pch.hpp)

find_package(argparse REQUIRED)
find_package(asio REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
find_package(stb REQUIRED)

target_include_directories(pine PUBLIC src)

target_link_libraries(pine
    PUBLIC 
        argparse::argparse
        asio::asio
        glm::glm
        imgui::imgui
        spdlog::spdlog
        pine::settings
    PRIVATE 
        glad::glad
        glfw
        stb::stb
        $<$<BOOL:${PINE_SANITIZERS_ENABLE}>:pine::options>
        $<$<BOOL:${PINE_WARNINGS_ENABLE}>:pine::warnings>
)

if(WIN32)
    target_link_libraries(pine PUBLIC wsock32 ws2_32)
    target_compile_definitions(pine PUBLIC PINE_PLATFORM_WINDOWS)
elseif(UNIX)
    target_compile_definitions(pine PUBLIC PINE_PLATFORM_LINUX)
endif()

target_compile_definitions(pine
    PUBLIC 
        GLFW_INCLUDE_NONE
        $<$<CONFIG:Debug>:PINE_DEBUG>
        $<$<CONFIG:Release>:PINE_RELEASE>
        $<$<BOOL:${PINE_GLFW_ENABLED}>:PINE_WINDOW_GLFW>
        $<$<BOOL:${PINE_OPENGL_ENABLED}>:PINE_RENDER_OPENGL>
    PRIVATE
)

set_target_properties(pine PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Install static libraries, shared libraries, and executables
install(TARGETS pine
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# Install header files in include directory
install(DIRECTORY "include/pine" TYPE INCLUDE FILES_MATCHING PATTERN "*.hpp")
