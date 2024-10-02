#ifndef PACKETLOGIN_HPP
#define PACKETLOGIN_HPP

#include "Packet.hpp"

class PacketLogin : public Packet {
public:
    PacketLogin(asio::ip::udp::socket& socket_, uint16_t id) : Packet(PacketFactory::TypePacket::LOGIN, id, socket_) {}

    ~PacketLogin() = default;

    void format_data(std::string username, std::string password);

    void format_data() override;
};

#endif //PACKETLOGIN_HPP
