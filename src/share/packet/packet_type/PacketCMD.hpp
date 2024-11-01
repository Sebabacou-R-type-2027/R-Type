#ifndef PACKETCMD_HPP
#define PACKETCMD_HPP

#include "Packet.hpp"

class PacketCMD : public Packet {
public:
    PacketCMD(asio::ip::udp::socket& socket_, uint16_t id) : Packet(PacketFactory::TypePacket::CMD, id, socket_) {}

    ~PacketCMD() = default;

    void format_data(std::string cmd);

    void format_data() override;
};

#endif //PACKETCMD_HPP
