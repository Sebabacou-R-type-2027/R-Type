#ifndef PACKETPING_HPP
#define PACKETPING_HPP

#include "Packet.hpp"

class PacketPing : public Packet {
    public:
        /// \brief Constructor for PacketPing class
        PacketPing(uint16_t id) : Packet(PacketFactory::PING, id) {}

        /// \brief Destructor for PacketPing class
        ~PacketPing() override = default;

        /// \brief Set the data of the packet
        /// \details Set the data of the packetPing to the current id;
        void format_data() override;
};

#endif //PACKETPING_HPP
