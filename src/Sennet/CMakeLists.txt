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
           include/Sennet/Renderer/Image.hpp
           include/Sennet/Renderer/OrthographicCamera.hpp
           include/Sennet/Renderer/OrthographicCameraController.hpp
           include/Sennet/Renderer/RenderCommand.hpp
           include/Sennet/Renderer/Renderer.hpp
           include/Sennet/Renderer/Renderer2D.hpp
           include/Sennet/Renderer/RendererAPI.hpp
           include/Sennet/Renderer/Shader.hpp
           include/Sennet/Renderer/Texture.hpp
           include/Sennet/Renderer/VertexArray.hpp
           include/Sennet/UI/UI.hpp
           include/Sennet/UI/ImGuiImplGLFW.hpp
           include/Sennet/UI/ImGuiImplOpenGL3.hpp
           include/Sennet/UI/ImGuiImplOpenGL3Loader.hpp
           include/Sennet/UI/ImGuiLayer.hpp
           include/Sennet/Utils/FileSystem.hpp
           include/Sennet/Utils/Math.hpp
    PRIVATE src/Core/Application.cpp
            src/Core/Layer.cpp
            src/Core/LayerStack.cpp
            src/Core/Log.cpp
            src/Network/Socket.cpp
            src/Platform/OpenGL/OpenGLBuffer.cpp
            src/Platform/OpenGL/OpenGLFramebuffer.cpp
            src/Platform/OpenGL/OpenGLContext.cpp
            src/Platform/OpenGL/OpenGLRendererAPI.cpp
            src/Platform/OpenGL/OpenGLShader.cpp
            src/Platform/OpenGL/OpenGLTexture.cpp
            src/Platform/OpenGL/OpenGLVertexArray.cpp
            src/Platform/Windows/WindowsInput.cpp
            src/Platform/Windows/WindowsWindow.cpp
            src/Platform/Linux/LinuxInput.cpp
            src/Platform/Linux/LinuxSocket.cpp
            src/Platform/Linux/LinuxWindow.cpp
            src/Renderer/Buffer.cpp
            src/Renderer/Framebuffer.cpp
            src/Renderer/GraphicsContext.cpp
            src/Renderer/Image.cpp
            src/Renderer/OrthographicCamera.cpp
            src/Renderer/OrthographicCameraController.cpp
            src/Renderer/RenderCommand.cpp
            src/Renderer/Renderer.cpp
            src/Renderer/Renderer2D.cpp
            src/Renderer/RendererAPI.cpp
            src/Renderer/Shader.cpp
            src/Renderer/Texture.cpp
            src/Renderer/VertexArray.cpp
            src/UI/UI.cpp
            src/UI/ImGuiImplGLFW.cpp
            src/UI/ImGuiImplOpenGL3.cpp
            src/UI/ImGuiLayer.cpp
            src/Utils/FileSystem.cpp)

target_precompile_headers(sennet PRIVATE include/Sennet/Pch.hpp)

find_package(asio REQUIRED)
find_package(EnTT REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)
find_package(stb REQUIRED)

target_include_directories(
    sennet
    PUBLIC include
    PRIVATE src)

target_link_libraries(
    sennet
    PUBLIC asio::asio
           glm::glm
           imgui::imgui
           spdlog::spdlog
    PRIVATE glad::glad
            glfw::glfw
            EnTT::EnTT
            stb::stb)

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

target_compile_features(sennet PUBLIC cxx_std_17)

target_compile_options(sennet PUBLIC -std=c++17)

set_target_properties(
    sennet
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
               LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
