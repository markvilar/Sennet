#include "pine/utils/argparse.hpp"

#include <vector>

#include "pine/pch.hpp"

namespace pine
{

ArgumentParser::ArgumentParser(const std::string& description) noexcept
    : impl(description)
{
}

bool ArgumentParser::parse_arguments(const int count, char** arguments)
{
    try
    {
        impl.parse_args(std::vector<std::string>(arguments, arguments + count));
    }
    catch (const std::runtime_error& error)
    {
        return false;
    }
    return true;
}

void parse_arguments(const int count, char** arguments, std::error_code& ec);

} // namespace pine
