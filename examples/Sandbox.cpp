#include <chrono>
#include <thread>

#include "Pine/Pine.hpp"
#include "Pine/Experimental.hpp"

static bool exitFlag = false;

void signalHandler(const int signum) { exitFlag = true; }

int main(int argc, char** argv) 
{ 
    signal(SIGINT, signalHandler);

    Pine::Log::Init();

    const auto host = "192.168.1.150";
    const auto port = 4001;
    Pine::UDP::ClientState client;
    if (!Pine::Connect(client, host, port))
    {
        PINE_INFO("Not able to connect to server {0}:{1}", host, port);
        return 1;
    }

    Pine::Send(client);
    Pine::Receive(client);

    while (!exitFlag)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    Pine::Disconnect(client);
}
