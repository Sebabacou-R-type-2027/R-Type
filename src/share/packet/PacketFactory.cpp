
#include "PacketFactory.hpp"
#include "PacketPing.hpp"
#include "PacketACK.hpp"
#include "PacketCMD.hpp"
#include "PacketCMDP.hpp"
#include "Packet.hpp"

uint16_t PacketFactory::id_ = 0;

std::unique_ptr<Packet> PacketFactory::create_packet(TypePacket type, asio::ip::udp::socket& socket_) {
    switch (type) {
        case PING:
            up_id();
            return std::make_unique<PacketPing>(socket_, id_);
        case ACK:
            up_id();
            return std::make_unique<PacketACK>(socket_, id_);
        case CMD:
            up_id();
            return std::make_unique<PacketCMD>(socket_, id_);
        case CMDP:
            up_id();
            return std::make_unique<PacketCMDP>(socket_, id_);
        default:
            throw PacketFactoryException("Unknown packet type", type);
    }
}

void PacketFactory::up_id() {
    if (id_ == 0xFFFF) {
        id_ = 0;
    } else {
        id_++;
    }
}