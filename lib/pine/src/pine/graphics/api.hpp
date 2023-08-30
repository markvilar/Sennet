#pragma once

#include "pine/platform/glad/loader.hpp"

namespace pine {

/*
Loading API:
 - load_graphics_api
 - graphics_api_loaded
*/

template <typename WindowContext>
bool load_graphics_api(const WindowContext& context) {
    return load_opengl(context);
};

bool graphics_api_loaded();


} // namespace pine
