#pragma once

#include <memory>

#include "pine/core/common.hpp"
#include "pine/renderer/camera.hpp"
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
    static void init();
    static void on_window_resize(const uint32_t width, const uint32_t height);

    static void begin_scene(const OrthographicCamera& camera);
    static void end_scene();

    static void submit(const Shader& shader, const VertexArray& vertexArray,
        const Mat4& transform = Mat4(1.0f));

    inline static RendererAPI::API get_api() { return RendererAPI::get_api(); }

private:
    struct SceneData
    {
        Mat4 view_projection_matrix;
    };

    static std::unique_ptr<SceneData> s_scene_data;
};

} // namespace pine
