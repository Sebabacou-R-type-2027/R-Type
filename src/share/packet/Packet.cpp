#include "Packet.hpp"

#include <iostream>
#include <ostream>

Packet::Packet(PacketFactory::TypePacket type, uint16_t id, asio::ip::udp::socket& socket) : type_(type), socket_(socket) {
    this->set_id(id);
}

void Packet::send_packet(const asio::ip::udp::endpoint& receiver) {
    std::vector<uint8_t> buffer;

    std::visit([&](auto&& arg) {
        if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, uint8_t>) {
            buffer.push_back(arg);
        } else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, uint16_t>) {
            buffer.push_back(static_cast<uint8_t>(arg >> 8));
            buffer.push_back(static_cast<uint8_t>(arg & 0xFF));
        } else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, uint24_t>) {
            buffer.push_back(arg.data[0]);
            buffer.push_back(arg.data[1]);
            buffer.push_back(arg.data[2]);
        }
    }, this->size_);

    std::visit([&](auto&& arg) {
        if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, uint8_t>) {
            buffer.push_back(arg);
        } else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, uint16_t>) {
            buffer.push_back(static_cast<uint8_t>(arg >> 8));
            buffer.push_back(static_cast<uint8_t>(arg & 0xFF));
        }
    }, this->idp_);

    buffer.push_back(this->type_);
    buffer.insert(buffer.end(), this->data_.begin(), this->data_.end());
    if (this->is_data_set_ == false) {
        throw PacketFactory::PacketFactoryException("Data not set", this->get_idp());
    }
    this->socket_.send_to(asio::buffer(buffer, 4), receiver);
}

uint32_t Packet::get_size() const {
    return std::visit([](auto&& arg) {
        if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, Packet::uint24_t>) {
            return (static_cast<uint32_t>(arg.data[0]) << 16) |
                   (static_cast<uint32_t>(arg.data[1]) << 8) |
                   static_cast<uint32_t>(arg.data[2]);
        } else {
            return static_cast<uint32_t>(arg);
        }
    }, this->size_);
}

uint16_t Packet::get_idp() const {
    return std::visit([](auto&& arg) {
        if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, uint8_t>) {
            return static_cast<uint16_t>(arg);
        } else {
            return arg;
        }
    }, this->idp_);
}

uint8_t Packet::get_type() const {
    return this->type_;
}

std::string Packet::get_data() const {
    return this->data_;
}

void Packet::set_size() {
    uint32_t size = 1;

    size += this->data_.size();
    std::visit([&](auto&& arg) {
        size += this->get_size_varint(static_cast<uint32_t>(arg));
    }, this->idp_);
    switch (this->get_size_varint(size)) {
        case 1:
            this->size_ = static_cast<uint8_t>(size);
            break;
        case 2:
            this->size_ = static_cast<uint16_t>(size);
            break;
        case 3:
            this->size_ = Packet::convert_to_uint24(size);
            break;
        default:
            throw PacketFactory::PacketFactoryException("Unknown size type", size);
    }
}

void Packet::set_id(uint16_t id) {
    switch (this->get_size_varint(id)) {
        case 1:
            this->idp_ = static_cast<uint8_t>(id);
            break;
        case 2:
            this->idp_ = id;
            break;
        default:
            throw PacketFactory::PacketFactoryException("Unknown id type", id);
    }
}

void Packet::set_type(uint8_t type) {
    this->type_ = type;
}

Packet::uint24_t Packet::convert_to_uint24(uint32_t number) {
    uint24_t result{};
    result.data[0] = static_cast<uint8_t>((number >> 16) & 0xFF);
    result.data[1] = static_cast<uint8_t>((number >> 8) & 0xFF);
    result.data[2] = static_cast<uint8_t>(number & 0xFF);
    return result;
}

int Packet::get_size_varint(uint32_t number) {
    if (number <= 0xFF) {
        return 1;
    }
    if (number <= 0xFFFF) {
        return 2;
    }
    if (number <= 0xFFFFFF) {
        return 3;
    }
    std::visit([](auto&& arg) {
        throw PacketFactory::PacketFactoryException("Unknown size type", static_cast<uint16_t>(arg));
    }, this->idp_);
    return 0;
}

std::ostream& operator<<(std::ostream& os, const Packet& packet) {
    os << "Packet: " << std::endl;
    os << "Size: " << packet.get_size() << std::endl;
    os << "Id: " << packet.get_idp() << std::endl;
    os << "Type: " << static_cast<int>(packet.get_type()) << std::endl;
    os << "Data: " << packet.get_data() << std::endl;
    return os;
}
