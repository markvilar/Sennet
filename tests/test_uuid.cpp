#include <array>
#include <iostream>

#include <pine/pine.hpp>

int test_uniqueness()
{
    std::array<pine::UUID, 10000> reference_uuids{};
    std::array<pine::UUID, 10000> index_uuids{};
    auto cumulative = 0;
    for (const auto& reference : reference_uuids)
    {
        for (const auto& index: index_uuids)
        {
            if (reference == index)
            {
                cumulative += 1;
            }
        }
    }
    return cumulative;
}

int main(int argc, char** argv)
{
    auto error = 0;
    error += test_uniqueness();
    return error;
}
