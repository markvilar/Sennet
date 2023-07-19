#pragma once

#include <cstdint>
#include <random>

namespace pine {

class UUID {
public:
    UUID();
    UUID(const uint64_t value);
    UUID(const UUID& other);

    bool operator==(const UUID& other) const;
    bool operator!=(const UUID& other) const;

    std::string to_string() const { return std::to_string(value); }

private:
    uint64_t value{};
};

} // namespace pine
