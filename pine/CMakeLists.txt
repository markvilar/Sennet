add_library(pine STATIC)
add_library(pine::pine ALIAS pine)

target_sources(
    pine
    PUBLIC include/Pine/Pine.hpp
           include/Pine/Core/Application.hpp
           include/Pine/Core/Assert.hpp
           include/Pine/Core/Base.hpp
           include/Pine/Core/Input.hpp
           include/Pine/Core/KeyCodes.hpp
           include/Pine/Core/Layer.hpp
           include/Pine/Core/LayerStack.hpp
           include/Pine/Core/Log.hpp
           include/Pine/Core/MouseCodes.hpp
           include/Pine/Core/Platform.hpp
           include/Pine/Core/Timestep.hpp
           include/Pine/Core/Window.hpp
           include/Pine/Debug/Instrumentor.hpp
           include/Pine/Events/ApplicationEvent.hpp
           include/Pine/Events/Event.hpp
           include/Pine/Events/KeyEvent.hpp
           include/Pine/Events/MouseEvent.hpp
           include/Pine/Network/Connection.hpp
           include/Pine/Network/Message.hpp
           include/Pine/Network/TCPClient.hpp
           include/Pine/Network/TCPServer.hpp
           include/Pine/Network/ThreadSafeQueue.hpp
           include/Pine/Platform/OpenGL/OpenGLBuffer.hpp
           include/Pine/Platform/OpenGL/OpenGLFramebuffer.hpp
           include/Pine/Platform/OpenGL/OpenGLContext.hpp
           include/Pine/Platform/OpenGL/OpenGLRendererAPI.hpp
           include/Pine/Platform/OpenGL/OpenGLShader.hpp
           include/Pine/Platform/OpenGL/OpenGLTexture.hpp
           include/Pine/Platform/OpenGL/OpenGLVertexArray.hpp
           include/Pine/Platform/Windows/WindowsWindow.hpp
           include/Pine/Platform/Linux/LinuxWindow.hpp
           include/Pine/Renderer/Buffer.hpp
           include/Pine/Renderer/Framebuffer.hpp
           include/Pine/Renderer/GraphicsContext.hpp
           include/Pine/Renderer/Image.hpp
           include/Pine/Renderer/OrthographicCamera.hpp
           include/Pine/Renderer/OrthographicCameraController.hpp
           include/Pine/Renderer/RenderCommand.hpp
           include/Pine/Renderer/Renderer.hpp
           include/Pine/Renderer/Renderer2D.hpp
           include/Pine/Renderer/RendererAPI.hpp
           include/Pine/Renderer/Shader.hpp
           include/Pine/Renderer/Texture.hpp
           include/Pine/Renderer/VertexArray.hpp
           include/Pine/UI/UI.hpp
           include/Pine/UI/ImGuiImplGLFW.hpp
           include/Pine/UI/ImGuiImplOpenGL3.hpp
           include/Pine/UI/ImGuiImplOpenGL3Loader.hpp
           include/Pine/UI/ImGuiLayer.hpp
           include/Pine/Utils/FileSystem.hpp
           include/Pine/Utils/Math.hpp
    PRIVATE src/Core/Application.cpp
            src/Core/Layer.cpp
            src/Core/LayerStack.cpp
            src/Core/Log.cpp
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

target_precompile_headers(pine PRIVATE include/Pine/Pch.hpp)

find_package(asio REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)
find_package(stb REQUIRED)

target_include_directories(
    pine
    PUBLIC include
    PRIVATE src)

target_link_libraries(
    pine
    PUBLIC asio::asio
           glm::glm
           imgui::imgui
           spdlog::spdlog
    PRIVATE glad::glad
            glfw::glfw
            stb::stb)

if(MSVC)
    target_compile_definitions(pine PUBLIC PINE_PLATFORM_WINDOWS)
elseif(UNIX)
    target_compile_definitions(pine PUBLIC PINE_PLATFORM_LINUX)
endif()

target_compile_definitions(
    pine
    PUBLIC IMGUI_IMPL_GLFW_OPENGL3
           IMGUI_IMPL_OPENGL_LOADER_GLAD
           GLFW_INCLUDE_NONE
           $<$<CONFIG:Debug>:PINE_DEBUG>
           $<$<CONFIG:Release>:PINE_RELEASE>
    PRIVATE)

target_compile_features(pine PUBLIC cxx_std_17)

target_compile_options(pine PUBLIC -std=c++17)

set_target_properties(
    pine
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
               LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_custom_command(TARGET pine 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources/"
        "${CMAKE_BINARY_DIR}/bin/resources/")
