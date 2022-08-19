#include <chrono>
#include <thread>

#include "pine/pine.hpp"

static bool exit_flag = false;

void signal_handler([[maybe_unused]] const int signum) { exit_flag = true; }

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    signal(SIGINT, signal_handler);

    pine::Log::init();

    // Sandbox for pines functionality.
}
