#ifndef PACKETACK_HPP
#define PACKETACK_HPP

#include "Packet.hpp"

class PacketACK : public Packet {
public:
    /// \brief Constructor for PacketACK class
    PacketACK(asio::ip::udp::socket& socket_, uint16_t id) : Packet(PacketFactory::TypePacket::PING, id, socket_) {}

    /// \brief Destructor for PacketACK class
    ~PacketACK() = default;

    /// \brief Set the data of the packet
    /// \details Set the data of the packetPing to the current id;
    void format_data(bool status);

    void format_data() override;
};

#endif //PACKETACK_HPP
