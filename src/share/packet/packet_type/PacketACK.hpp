#ifndef PACKETACK_HPP
#define PACKETACK_HPP

#include "Packet.hpp"

class PacketACK : public Packet {
public:
    PacketACK(asio::ip::udp::socket& socket_, uint16_t id) : Packet(PacketFactory::TypePacket::ACK, id, socket_) {}

    ~PacketACK() = default;

    void format_data(bool status);

    void format_data() override;
};

#endif //PACKETACK_HPP
