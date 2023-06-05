#include "pine/utils/uuid.hpp"

namespace pine
{

static std::random_device device;
static std::mt19937_64 generator(device());
static std::uniform_int_distribution<uint64_t> uniform_dist;

UUID::UUID()
    : value(uniform_dist(generator))
{
}

UUID::UUID(const uint64_t value_)
    : value(value_)
{
}

UUID::UUID(const UUID& other)
    : value(other.value)
{
}

bool UUID::operator==(const UUID& other) const
{
    return value == other.value;
}

bool UUID::operator!=(const UUID& other) const
{
    return value != other.value;
}

} // namespace pine
