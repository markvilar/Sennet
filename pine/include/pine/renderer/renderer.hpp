#pragma once

#include <memory>

#include "pine/core/common.hpp"
#include "pine/renderer/orthographic_camera.hpp"
#include "pine/renderer/renderer_api.hpp"
#include "pine/renderer/shader.hpp"
#include "pine/renderer/texture.hpp"
#include "pine/utils/math.hpp"

namespace pine
{

class ShaderLibrary;

class Renderer
{
public:
    static void Init();
    static void OnWindowResize(const uint32_t width, const uint32_t height);

    static void BeginScene(const OrthographicCamera& camera);
    static void EndScene();

    static void Submit(const Shader& shader, const VertexArray& vertexArray,
        const Mat4& transform = Mat4(1.0f));

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    struct SceneData
    {
        Mat4 ViewProjectionMatrix;
    };

    static std::unique_ptr<SceneData> s_SceneData;
};

} // namespace pine
