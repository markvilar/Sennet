#pragma once

#include "Pine/Core/Base.hpp"

namespace Pine
{

class GraphicsContext
{
public:
    virtual ~GraphicsContext() = default;

    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;

    static GraphicsContext* Create(void* window);
};

} // namespace Pine
