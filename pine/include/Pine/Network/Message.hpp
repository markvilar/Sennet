#pragma once

namespace Pine
{

struct MessageHeader
{
    uint64_t Size = 0;
};

struct Message
{
    MessageHeader Header{};
    std::vector<uint8_t> Body{};
};

} // namespace Pine
