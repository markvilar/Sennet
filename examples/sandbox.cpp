#include <chrono>
#include <thread>

#include "pine/pine.hpp"

static bool exitFlag = false;

void signalHandler(const int signum) { exitFlag = true; }

int main(int argc, char** argv)
{
    signal(SIGINT, signalHandler);

    pine::Log::init();

    // Sandbox for pines functionality.
}
