#include "pine/pine.hpp"

#include <chrono>
#include <thread>

static bool exit_flag = false;

void signal_handler([[maybe_unused]] const int signum) { exit_flag = true; }

int main(int argc, char** argv)
{
    signal(SIGINT, signal_handler);

    pine::Log::init();

    // Set up argument parser
    auto parser
        = pine::ArgumentParser("Sandbox executable for testing of features.");

    // Add some positional arguments
    parser.add_positional<'d', int>("first", "the first argument");
    parser.add_positional<'d', int>("second", "the second argument");

    // Add positional argument with default value
    parser.add_positional<std::string>("file", "test.txt", "name of a file");

    // Add flag with default and implicit value
    parser.add_flag<std::string>("-flag",
        "hello",
        "hello world",
        "a flag argument is 'hello' or 'hello world'");

    // Parse arguments
    const auto success = parser.parse_arguments(argc, argv);
    if (!success)
    {
        PINE_CORE_ERROR("Failed to parse arguments!");
        exit(1);
    }

    const auto first = parser.get<int>("first");
    const auto second = parser.get<int>("second");
    const auto file = parser.get<std::string>("file");
    const auto flag = parser.get<std::string>("-flag");

    PINE_CORE_INFO("a:        {0}", first);
    PINE_CORE_INFO("b:        {0}", second);
    PINE_CORE_INFO("file:     {0}", file);
    PINE_CORE_INFO("flag:     {0}", flag);
}
