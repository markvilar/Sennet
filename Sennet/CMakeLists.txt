set(SENNET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Sennet/Include")
set(SENNET_SOURCE_DIR "${PROJECT_SOURCE_DIR}/Sennet/Source")
set(SENNET_VENDOR_DIR "${PROJECT_SOURCE_DIR}/Sennet/Vendor")

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

add_subdirectory(Vendor)

if(SENNET_BUILD_EXAMPLE)
    add_subdirectory(Example)
endif()

message(STATUS "Conan asio:   ${CONAN_INCLUDE_DIRS_ASIO}"  )
message(STATUS "Conan glm:    ${CONAN_INCLUDE_DIRS_GLM}"   )
message(STATUS "Conan spdlog: ${CONAN_INCLUDE_DIRS_SPDLOG}")
message(STATUS "Conan glad:   ${CONAN_INCLUDE_DIRS_GLAD}"  )
message(STATUS "Conan glfw:   ${CONAN_INCLUDE_DIRS_GLFW}"  )

target_include_directories(
    Sennet
    PUBLIC "${SENNET_INCLUDE_DIR}"
            "${CONAN_INCLUDE_DIRS_ASIO}"
           "${CONAN_INCLUDE_DIRS_GLM}"
            "${SENNET_VENDOR_DIR}/imgui"            
            "${CONAN_INCLUDE_DIRS_SPDLOG}"
    PRIVATE "${SENNET_SOURCE_DIR}"
            "${SENNET_VENDOR_DIR}/glad/include"
            "${SENNET_VENDOR_DIR}/glfw/include"
            "${SENNET_VENDOR_DIR}/stb")

target_link_libraries(
    Sennet
    PUBLIC imgui CONAN_PKG::glm CONAN_PKG::spdlog
    PRIVATE glad glfw)

# TODO: Replace with generator expression?
if(MSVC)
    target_compile_definitions(
        Sennet
        PUBLIC "-DSN_PLATFORM_WINDOWS"
                "$<$<CONFIG:Debug>:SN_DEBUG=1>"
                "$<$<CONFIG:Release>:SN_RELEASE=1>"
               "-DGLFW_INCLUDE_NONE"
        PRIVATE)
elseif(UNIX)
    target_compile_definitions(
        Sennet
        PUBLIC "-DSN_PLATFORM_LINUX"
                "$<$<CONFIG:Debug>:SN_DEBUG=1>"
                "$<$<CONFIG:Release>:SN_RELEASE=1>"
               "-DGLFW_INCLUDE_NONE"
        PRIVATE)

endif()

target_compile_features(Sennet PUBLIC cxx_std_20)
