#include "pine/core/input.hpp"

#include "pine/platform/linux/input.hpp"

namespace pine
{

std::unique_ptr<InputHandle> InputHandle::create(const Window& window)
{
#if defined(PINE_PLATFORM_LINUX)
    return std::make_unique<LinuxInputHandle>(window);
#endif
}

} // namespace pine
