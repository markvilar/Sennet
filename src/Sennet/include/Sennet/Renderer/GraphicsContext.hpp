#pragma once

#include "Sennet/Core/Base.hpp"

namespace Sennet
{

class GraphicsContext
{
public:
    virtual ~GraphicsContext() = default;

    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;

    static Scope<GraphicsContext> Create(void* window);
};

} // namespace Sennet
