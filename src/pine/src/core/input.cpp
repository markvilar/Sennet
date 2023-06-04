#include "input.hpp"

#include "../platform/linux/input.hpp"
#include "../platform/windows/input.hpp"

namespace pine
{

std::unique_ptr<InputHandle> InputHandle::create([[maybe_unused]]const Window& window)
{
#if defined(PINE_PLATFORM_LINUX)
    return std::make_unique<LinuxInputHandle>(window);
#endif
    PINE_CORE_ASSERT(false, "no input handle supported for this platform");
    return nullptr;
}

} // namespace pine
