#include <csignal>

#include "pine/pine.hpp"

static bool exit_flag = false;

void signal_handler([[maybe_unused]] const int signum) { exit_flag = true; }

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    signal(SIGINT, signal_handler);

    pine::Log::init();

    pine::ServerState server(6000);

    server.set_connection_callback(
        [](const pine::ConnectionState& connection) -> bool
        {
            PINE_INFO("Server got connection: {0}",
                connection.socket.remote_endpoint());
            return true;
        });

    server.set_message_callback(
        [](const std::vector<uint8_t>& message) -> void
        {
            PINE_INFO("Server got message: {0} : {1}",
                message.size(),
                std::string(message.begin(), message.end()));
        });

    
    pine::start_server(server);

    while (!exit_flag)
    {
        pine::update_server(server);
    }

    PINE_INFO("Stopping server.");
    pine::stop_server(server);

    return 0;
}
