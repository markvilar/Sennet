add_library(pine STATIC)
add_library(pine::pine ALIAS pine)

target_sources(pine
    PUBLIC 
        # Definitions and macros
        include/pine/assert.hpp
        include/pine/common.hpp
        include/pine/platform.hpp
        include/pine/types.hpp


    PRIVATE
        # Core
        src/core/application.cpp
        src/core/application.hpp
        src/core/input.cpp
        src/core/input.hpp
        src/core/key_codes.hpp
        src/core/layer.cpp
        src/core/layer.hpp
        src/core/log.cpp
        src/core/log.hpp
        src/core/mouse_codes.hpp
        src/core/timestep.hpp
        src/core/window.hpp

        # Events
        src/events/application_event.hpp
        src/events/event.hpp
        src/events/key_event.hpp
        src/events/mouse_event.hpp
       
        # Gui
        src/gui/common.hpp
        src/gui/manager.cpp
        src/gui/manager.hpp
        src/gui/style.cpp
        src/gui/style.hpp
        src/gui/widgets.hpp
        src/gui/window.cpp
        src/gui/window.hpp

        # Network
        src/network/client.cpp
        src/network/client.hpp
        src/network/connection.cpp
        src/network/connection.hpp
        src/network/server.cpp
        src/network/server.hpp
        src/network/types.hpp

        # Platform - OpenGL
        src/platform/opengl/buffer.cpp
        src/platform/opengl/buffer.hpp
        src/platform/opengl/context.cpp
        src/platform/opengl/context.hpp
        src/platform/opengl/framebuffer.cpp
        src/platform/opengl/framebuffer.hpp
        src/platform/opengl/renderer_api.cpp
        src/platform/opengl/renderer_api.hpp
        src/platform/opengl/shader.cpp
        src/platform/opengl/shader.hpp
        src/platform/opengl/texture.cpp
        src/platform/opengl/texture.hpp
        src/platform/opengl/utilities.cpp
        src/platform/opengl/utilities.hpp

        # Platform - Linux
        src/platform/linux/input.cpp
        src/platform/linux/input.hpp
        src/platform/linux/window.cpp
        src/platform/linux/window.hpp
        
        # Platform - Windows
        src/platform/windows/input.cpp
        src/platform/windows/input.hpp
        src/platform/windows/window.cpp
        src/platform/windows/window.hpp

        # Renderer
        src/renderer/buffer.cpp
        src/renderer/buffer.hpp
        src/renderer/camera.cpp
        src/renderer/camera.hpp
        src/renderer/framebuffer.cpp
        src/renderer/framebuffer.hpp
        src/renderer/graphics_context.cpp
        src/renderer/graphics_context.hpp
        src/renderer/image.cpp
        src/renderer/image.hpp
        src/renderer/quad_renderer.cpp
        src/renderer/quad_renderer.hpp
        src/renderer/render_command.cpp
        src/renderer/render_command.hpp
        src/renderer/renderer.cpp
        src/renderer/renderer.hpp
        src/renderer/renderer_api.cpp
        src/renderer/renderer_api.hpp
        src/renderer/shader.cpp
        src/renderer/shader.hpp
        src/renderer/texture.cpp
        src/renderer/texture.hpp
        src/renderer/types.cpp
        src/renderer/types.hpp

        # Utils
        src/utils/argparse.cpp
        src/utils/argparse.hpp
        src/utils/fonts.hpp
        src/utils/filesystem.cpp
        src/utils/filesystem.hpp
        src/utils/locked_queue.hpp
        src/utils/math.hpp
        src/utils/uuid.cpp
        src/utils/uuid.hpp
)

target_precompile_headers(pine PRIVATE include/pine/pch.hpp)

find_package(argparse REQUIRED)
find_package(asio REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
find_package(stb REQUIRED)

target_include_directories(pine PUBLIC include)

target_link_libraries(pine
    PUBLIC 
        argparse::argparse
        asio::asio
        glm::glm
        imgui::imgui
        spdlog::spdlog
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
    PRIVATE
)

target_compile_features(pine PUBLIC cxx_std_17)
target_compile_options(pine PUBLIC -std=c++17)

set_target_properties(pine PROPERTIES 
    CXX_STANDARD 17
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
