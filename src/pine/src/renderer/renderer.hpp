#pragma once

#include "camera.hpp"
#include "renderer_api.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include "pine/common.hpp"

#include "../utils/math.hpp"

#include <memory>

namespace pine
{

class Renderer
{
public:
    static void init();
    static void on_window_resize(const uint32_t width, const uint32_t height);

    static void begin_scene(const OrthographicCamera& camera);
    static void end_scene();

    static void submit(const Shader& shader,
        const VertexArray& vertexArray,
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
