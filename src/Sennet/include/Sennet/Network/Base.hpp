#pragma once

namespace Sennet
{

class NetworkAddress
{
public:
    NetworkAddress() = default;
    ~NetworkAddress() = default;

private:
    std::string m_Address;
    std::uint16_t m_Port;
};

} // namespace Sennet
