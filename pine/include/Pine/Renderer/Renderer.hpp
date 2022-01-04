#pragma once

#include "Pine/Core/Base.hpp"
#include "Pine/Renderer/OrthographicCamera.hpp"
#include "Pine/Renderer/RendererAPI.hpp"
#include "Pine/Renderer/Shader.hpp"
#include "Pine/Renderer/Texture.hpp"
#include "Pine/Utils/Math.hpp"

namespace Pine
{

class ShaderLibrary;

class Renderer
{
public:
    static void Init();
    static void OnWindowResize(const uint32_t width, const uint32_t height);

    static Ref<ShaderLibrary> GetShaderLibrary();

    static Ref<Texture2D> GetBlackTexture();
    static Ref<Texture2D> GetWhiteTexture();

    static void BeginScene(const OrthographicCamera& camera);
    static void EndScene();

    static void Submit(const Ref<Shader>& shader,
        const Ref<VertexArray>& vertexArray,
        const Mat4& transform = Mat4(1.0f));

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    struct SceneData
    {
        Mat4 ViewProjectionMatrix;
    };

    static Scope<SceneData> s_SceneData;
};

} // namespace Pine
