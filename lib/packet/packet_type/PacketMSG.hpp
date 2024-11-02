#ifndef PACKETMSG_HPP
#define PACKETMSG_HPP

#include "Packet.hpp"

class PacketMSG : public Packet {
public:
    PacketMSG(asio::ip::udp::socket& socket_, uint16_t id) : Packet(PacketFactory::TypePacket::MSG, id, socket_) {}

    ~PacketMSG() = default;

    void format_data(std::string cmd);

    void format_data() override;
};

#endif //PACKETMSG_HPP
