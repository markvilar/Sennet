#include <csignal>

#include "Pine/Pine.hpp"

static bool exitFlag = false;

void signalHandler(const int signum) { exitFlag = true; }

int main(int argc, char** argv)
{
    signal(SIGINT, signalHandler);

    Pine::Log::Init();

    Pine::ServerState server(6000);

    Pine::StartServer(server,
        [](const Pine::ConnectionState& connection) -> bool
        {
            PINE_INFO("Server got connection: {0}",
                connection.socket.remote_endpoint());
            return true;
        });

    while (!exitFlag)
    {
        Pine::UpdateServer(server,
            [](const std::vector<uint8_t>& message) -> void
            {
                PINE_INFO("Server got message: {0} : {1}",
                    message.size(),
                    std::string(message.begin(), message.end()));
            });
    }

    PINE_INFO("Stopping server.");
    Pine::StopServer(server);

    return 0;
}
