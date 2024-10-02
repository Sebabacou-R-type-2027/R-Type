#ifndef PACKETLOGOUT_HPP
#define PACKETLOGOUT_HPP

#include "Packet.hpp"

class PacketLogout : public Packet {
public:
    PacketLogout(asio::ip::udp::socket& socket_, uint16_t id) : Packet(PacketFactory::TypePacket::LOGOUT, id, socket_) {}

    ~PacketLogout() = default;

    void format_data(uint32_t id);

    void format_data() override;
};

#endif //PACKETLOGOUT_HPP
