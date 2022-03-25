#include <chrono>
#include <thread>

#include "Pine/Pine.hpp"

static bool exitFlag = false;

void signalHandler(const int signum) { exitFlag = true; }

int main(int argc, char** argv)
{
    signal(SIGINT, signalHandler);

    Pine::Log::Init();

    // Sandbox for Pines functionality.
}
