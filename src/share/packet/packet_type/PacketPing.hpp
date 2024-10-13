#ifndef PACKETPING_HPP
#define PACKETPING_HPP

#include "Packet.hpp"

class PacketPing : public Packet {
    public:
        /// \brief Constructor for PacketPing class
        PacketPing(asio::ip::udp::socket& socket_, uint16_t id) : Packet(PacketFactory::TypePacket::PING, id, socket_) {}

        /// \brief Destructor for PacketPing class
        ~PacketPing() = default;

        /// \brief Set the data of the packet
        /// \details Set the data of the packetPing to the current id;
        void format_data() override;
};

#endif //PACKETPING_HPP
