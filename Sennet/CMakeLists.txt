set(SENNET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Sennet/Include")
set(SENNET_SOURCE_DIR "${PROJECT_SOURCE_DIR}/Sennet/Source")

set(SENNET_HDR
    "${SENNET_INCLUDE_DIR}/Sennet/Sennet.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Core/Application.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Core/Base.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Core/Input.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Core/KeyCodes.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Core/Layer.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Core/LayerStack.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Core/Log.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Core/MouseCodes.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Core/Timestep.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Core/Window.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Debug/Instrumentor.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Events/ApplicationEvent.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Events/Event.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Events/KeyEvent.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Events/MouseEvent.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/ImGui/ImGuiLayer.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/ImGui/ImGuiUtilities.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Network/Base.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Network/Socket.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Network/ThreadSafeQueue.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Network/TCP/Message.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Network/TCP/Client.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Network/TCP/Connection.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Network/TCP/Server.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/Buffer.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/Framebuffer.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/GraphicsContext.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/OrthographicCamera.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/OrthographicCameraController.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/RenderCommand.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/Renderer.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/Renderer2D.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/RendererAPI.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/Shader.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/Texture.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Renderer/VertexArray.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Synchronization/Synchronizer.hpp"
    "${SENNET_INCLUDE_DIR}/Sennet/Synchronization/Timestamp.hpp")

set(SENNET_SRC
    "${SENNET_SOURCE_DIR}/Sennet/Core/Application.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Core/Layer.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Core/LayerStack.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Core/Log.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Core/Window.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/ImGui/ImGuiBuild.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/ImGui/ImGuiLayer.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/ImGui/ImGuiUtilities.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Network/Socket.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/Buffer.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/Framebuffer.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/GraphicsContext.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/OrthographicCamera.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/OrthographicCameraController.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/RenderCommand.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/Renderer.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/Renderer2D.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/RendererAPI.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/Shader.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/Texture.cpp"
    "${SENNET_SOURCE_DIR}/Sennet/Renderer/VertexArray.cpp")

set(SENNET_PLATFORM_HDR
    "${SENNET_INCLUDE_DIR}/Platform/OpenGL/OpenGLBuffer.hpp"
    "${SENNET_INCLUDE_DIR}/Platform/OpenGL/OpenGLFramebuffer.hpp"
    "${SENNET_INCLUDE_DIR}/Platform/OpenGL/OpenGLContext.hpp"
    "${SENNET_INCLUDE_DIR}/Platform/OpenGL/OpenGLRendererAPI.hpp"
    "${SENNET_INCLUDE_DIR}/Platform/OpenGL/OpenGLShader.hpp"
    "${SENNET_INCLUDE_DIR}/Platform/OpenGL/OpenGLTexture.hpp"
    "${SENNET_INCLUDE_DIR}/Platform/OpenGL/OpenGLVertexArray.hpp"
    "${SENNET_INCLUDE_DIR}/Platform/Windows/WindowsWindow.hpp"
    "${SENNET_INCLUDE_DIR}/Platform/Linux/LinuxSocket.hpp"
    "${SENNET_INCLUDE_DIR}/Platform/Linux/LinuxWindow.hpp")

set(SENNET_PLATFORM_SRC
    "${SENNET_SOURCE_DIR}/Platform/OpenGL/OpenGLBuffer.cpp"
    "${SENNET_SOURCE_DIR}/Platform/OpenGL/OpenGLFramebuffer.cpp"
    "${SENNET_SOURCE_DIR}/Platform/OpenGL/OpenGLContext.cpp"
    "${SENNET_SOURCE_DIR}/Platform/OpenGL/OpenGLRendererAPI.cpp"
    "${SENNET_SOURCE_DIR}/Platform/OpenGL/OpenGLShader.cpp"
    "${SENNET_SOURCE_DIR}/Platform/OpenGL/OpenGLTexture.cpp"
    "${SENNET_SOURCE_DIR}/Platform/OpenGL/OpenGLVertexArray.cpp"
    "${SENNET_SOURCE_DIR}/Platform/Windows/WindowsInput.cpp"
    "${SENNET_SOURCE_DIR}/Platform/Windows/WindowsWindow.cpp"
    "${SENNET_SOURCE_DIR}/Platform/Linux/LinuxInput.cpp"
    "${SENNET_SOURCE_DIR}/Platform/Linux/LinuxSocket.cpp"
    "${SENNET_SOURCE_DIR}/Platform/Linux/LinuxWindow.cpp")

add_library(
    Sennet STATIC
    ${SENNET_HDR}
    ${SENNET_SRC}
    ${SENNET_PLATFORM_HDR}
    ${SENNET_PLATFORM_SRC})

add_library(Sennet::Sennet ALIAS Sennet)

target_precompile_headers(Sennet PRIVATE "${SENNET_INCLUDE_DIR}/Sennet/Pch.hpp")

if(SENNET_BUILD_EXAMPLE)
    add_subdirectory(Example)
endif()

add_subdirectory(Vendor)

# TODO: Add find_package() support for imgui.
find_package(asio REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)
find_package(stb REQUIRED)

target_include_directories(
    Sennet
    PUBLIC ${SENNET_INCLUDE_DIR} ${glm_INCLUDE_DIRS}
    PRIVATE ${SENNET_SOURCE_DIR})

target_link_libraries(
    Sennet
    PUBLIC asio::asio imgui::imgui glm::glm spdlog::spdlog
    PRIVATE glad::glad glfw::glfw stb::stb)

# TODO: Replace with generator expression?
if(MSVC)
    target_compile_definitions(
        Sennet
        PUBLIC SENNET_PLATFORM_WINDOWS
               IMGUI_IMPL_GLFW_OPENGL3
               IMGUI_IMPL_OPENGL_LOADER_GLAD
               GLFW_INCLUDE_NONE
               "$<$<CONFIG:Debug>:SENNET_DEBUG>"
               "$<$<CONFIG:Release>:SENNET_RELEASE>"
        PRIVATE)
elseif(UNIX)
    target_compile_definitions(
        Sennet
        PUBLIC SENNET_PLATFORM_LINUX
               IMGUI_IMPL_GLFW_OPENGL3
               IMGUI_IMPL_OPENGL_LOADER_GLAD
               GLFW_INCLUDE_NONE
               "$<$<CONFIG:Debug>:SENNET_DEBUG>"
               "$<$<CONFIG:Release>:SENNET_RELEASE>"
        PRIVATE)
endif()

target_compile_features(Sennet PUBLIC cxx_std_20)