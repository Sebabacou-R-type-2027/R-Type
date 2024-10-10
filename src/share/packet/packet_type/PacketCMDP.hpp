#ifndef PACKETCMDP_HPP
#define PACKETCMDP_HPP

#include "Packet.hpp"

class PacketCMDP : public Packet {
public:
    PacketCMDP(asio::ip::udp::socket& socket_, uint16_t id) : Packet(PacketFactory::TypePacket::CMDP, id, socket_) {}

    ~PacketCMDP() = default;

    void format_data(uint8_t status);

    void format_data() override;
};

#endif //PACKETCMDP_HPP
