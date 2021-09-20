#include "Sennet/Pch.hpp"

#if not defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    #define IMGUI_IMPL_OPENGL_LOADER_GLAD
#endif

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
