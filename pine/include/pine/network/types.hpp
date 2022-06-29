#pragma once

#include <asio.hpp>

namespace pine
{

using NetworkContext = asio::io_context;

using AcceptorType = asio::ip::tcp::acceptor;
using EndpointType = asio::ip::tcp::endpoint;
using SocketType = asio::ip::tcp::socket;
using Resolver = asio::ip::tcp::resolver;
using ResolveType = asio::ip::tcp::resolver::results_type;

}; // namespace pine
