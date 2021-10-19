add_library(sennet STATIC)
add_library(sennet::sennet ALIAS sennet)

target_sources(
    sennet
    PUBLIC include/Sennet/Sennet.hpp
           include/Sennet/Core/Application.hpp
           include/Sennet/Core/Assert.hpp
           include/Sennet/Core/Base.hpp
           include/Sennet/Core/Input.hpp
           include/Sennet/Core/KeyCodes.hpp
           include/Sennet/Core/Layer.hpp
           include/Sennet/Core/LayerStack.hpp
           include/Sennet/Core/Log.hpp
           include/Sennet/Core/MouseCodes.hpp
           include/Sennet/Core/Platform.hpp
           include/Sennet/Core/Timestep.hpp
           include/Sennet/Core/Window.hpp
           include/Sennet/Debug/Instrumentor.hpp
           include/Sennet/Events/ApplicationEvent.hpp
           include/Sennet/Events/Event.hpp
           include/Sennet/Events/KeyEvent.hpp
           include/Sennet/Events/MouseEvent.hpp
           include/Sennet/ImGui/imgui_impl_glfw.h
           include/Sennet/ImGui/imgui_impl_opengl3.h
           include/Sennet/ImGui/imgui_impl_opengl3_loader.h
           include/Sennet/ImGui/ImGuiBuild.hpp
           include/Sennet/ImGui/ImGuiHelpers.hpp
           include/Sennet/ImGui/ImGuiLayer.hpp
           include/Sennet/Network/Base.hpp
           include/Sennet/Network/Socket.hpp
           include/Sennet/Network/ThreadSafeQueue.hpp
           include/Sennet/Network/TCP/Message.hpp
           include/Sennet/Network/TCP/Client.hpp
           include/Sennet/Network/TCP/Connection.hpp
           include/Sennet/Network/TCP/Server.hpp
           include/Sennet/Platform/OpenGL/OpenGLBuffer.hpp
           include/Sennet/Platform/OpenGL/OpenGLFramebuffer.hpp
           include/Sennet/Platform/OpenGL/OpenGLContext.hpp
           include/Sennet/Platform/OpenGL/OpenGLRendererAPI.hpp
           include/Sennet/Platform/OpenGL/OpenGLShader.hpp
           include/Sennet/Platform/OpenGL/OpenGLTexture.hpp
           include/Sennet/Platform/OpenGL/OpenGLVertexArray.hpp
           include/Sennet/Platform/Windows/WindowsWindow.hpp
           include/Sennet/Platform/Linux/LinuxSocket.hpp
           include/Sennet/Platform/Linux/LinuxWindow.hpp
           include/Sennet/Renderer/Buffer.hpp
           include/Sennet/Renderer/Framebuffer.hpp
           include/Sennet/Renderer/GraphicsContext.hpp
           include/Sennet/Renderer/OrthographicCamera.hpp
           include/Sennet/Renderer/OrthographicCameraController.hpp
           include/Sennet/Renderer/RenderCommand.hpp
           include/Sennet/Renderer/Renderer.hpp
           include/Sennet/Renderer/Renderer2D.hpp
           include/Sennet/Renderer/RendererAPI.hpp
           include/Sennet/Renderer/Shader.hpp
           include/Sennet/Renderer/Texture.hpp
           include/Sennet/Renderer/VertexArray.hpp
    PRIVATE src/Sennet/Core/Application.cpp
            src/Sennet/Core/Layer.cpp
            src/Sennet/Core/LayerStack.cpp
            src/Sennet/Core/Log.cpp
            src/Sennet/ImGui/imgui_impl_glfw.cpp
            src/Sennet/ImGui/imgui_impl_opengl3.cpp
            src/Sennet/ImGui/ImGuiHelpers.cpp
            src/Sennet/ImGui/ImGuiLayer.cpp
            src/Sennet/Network/Socket.cpp
            src/Sennet/Platform/OpenGL/OpenGLBuffer.cpp
            src/Sennet/Platform/OpenGL/OpenGLFramebuffer.cpp
            src/Sennet/Platform/OpenGL/OpenGLContext.cpp
            src/Sennet/Platform/OpenGL/OpenGLRendererAPI.cpp
            src/Sennet/Platform/OpenGL/OpenGLShader.cpp
            src/Sennet/Platform/OpenGL/OpenGLTexture.cpp
            src/Sennet/Platform/OpenGL/OpenGLVertexArray.cpp
            src/Sennet/Platform/Windows/WindowsInput.cpp
            src/Sennet/Platform/Windows/WindowsWindow.cpp
            src/Sennet/Platform/Linux/LinuxInput.cpp
            src/Sennet/Platform/Linux/LinuxSocket.cpp
            src/Sennet/Platform/Linux/LinuxWindow.cpp
            src/Sennet/Renderer/Buffer.cpp
            src/Sennet/Renderer/Framebuffer.cpp
            src/Sennet/Renderer/GraphicsContext.cpp
            src/Sennet/Renderer/OrthographicCamera.cpp
            src/Sennet/Renderer/OrthographicCameraController.cpp
            src/Sennet/Renderer/RenderCommand.cpp
            src/Sennet/Renderer/Renderer.cpp
            src/Sennet/Renderer/Renderer2D.cpp
            src/Sennet/Renderer/RendererAPI.cpp
            src/Sennet/Renderer/Shader.cpp
            src/Sennet/Renderer/Texture.cpp
            src/Sennet/Renderer/VertexArray.cpp)

target_precompile_headers(sennet PRIVATE include/Sennet/Pch.hpp)

find_package(asio REQUIRED)
find_package(EnTT REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)

target_include_directories(
    sennet
    PUBLIC include ${glm_INCLUDE_DIRS}
    PRIVATE src)

target_link_libraries(
    sennet
    PUBLIC asio::asio
           imgui::imgui
           glm::glm
           spdlog::spdlog
    PRIVATE glad::glad glfw::glfw EnTT::EnTT)

if(MSVC)
    target_compile_definitions(sennet PUBLIC SENNET_PLATFORM_WINDOWS)
elseif(UNIX)
    target_compile_definitions(sennet PUBLIC SENNET_PLATFORM_LINUX)
endif()

target_compile_definitions(
    sennet
    PUBLIC IMGUI_IMPL_GLFW_OPENGL3
           IMGUI_IMPL_OPENGL_LOADER_GLAD
           GLFW_INCLUDE_NONE
           $<$<CONFIG:Debug>:SENNET_DEBUG>
           $<$<CONFIG:Release>:SENNET_RELEASE>
    PRIVATE)

target_compile_features(sennet PUBLIC cxx_std_20)
