#include "Packet.hpp"

#include <ostream>
#include <iostream>

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
    this->socket_.send_to(asio::buffer(buffer, buffer.size()), receiver);
}

std::tuple<unsigned, unsigned, unsigned, std::vector<uint8_t>> Packet::extract_packet(
    const char buffer[65535], size_t size) {
    uint32_t size_s = 0;
    uint32_t id = 0;
    uint32_t type = 0;
    uint32_t nbr_bytes_id = 0;
    uint32_t nbr_bytes_size = 0;

    if (size < 3) {
        throw PacketFactory::PacketFactoryException("Packet too small", 0);
    }
    if (size > 65535) {
        throw PacketFactory::PacketFactoryException("Packet too big", 0);
    }

    if (size - 3 <= 0xFF) {
        nbr_bytes_size = 1;
        size_s = static_cast<uint8_t>(buffer[0]);
    } else if (size - 3 <= 0xFFFF) {
        nbr_bytes_size = 2;
        size_s = (static_cast<uint8_t>(buffer[0]) << 8 | static_cast<uint8_t>(buffer[1]));
    } else {
        size_s = (static_cast<uint32_t>(static_cast<uint8_t>(buffer[0]) << 16) | static_cast<uint32_t>(static_cast<uint8_t>(buffer[1]) << 8) | static_cast<uint8_t>(buffer[2]));
    }
    switch (size - size_s - nbr_bytes_size - 1) {
    case 0:
        throw PacketFactory::PacketFactoryException("No ID", 0);
    case 1:
        nbr_bytes_id = 1;
        id = static_cast<uint8_t>(buffer[nbr_bytes_size]);
        break;
    case 2:
        nbr_bytes_id = 2;
        id = (static_cast<uint8_t>(buffer[nbr_bytes_size]) << 8 | static_cast<uint8_t>(buffer[nbr_bytes_size + 1]));
        break;
    default:
        throw PacketFactory::PacketFactoryException("Unknown ID size", 0);
    }
    type = static_cast<uint8_t>(buffer[nbr_bytes_size + nbr_bytes_id]);

    std::vector<uint8_t> remaining_data(buffer + nbr_bytes_size + nbr_bytes_id + 1, buffer + size);

    return std::make_tuple(size_s, id, type, remaining_data);
}

std::string Packet::extract_data(const char buffer[65535], size_t size, uint32_t type) {
    std::tuple<unsigned, unsigned, unsigned, std::vector<uint8_t>> packet = extract_packet(buffer, size);
    std::vector<uint8_t> data = std::get<3>(packet);
    std::string result;

    switch (type) {
        case PacketFactory::TypePacket::ACK || PacketFactory::TypePacket::CMDP:
            std::cout << "Data extract = " << type << std::endl;
            result.reserve(data.size());
            for (const auto& byte : data) {
                result += std::to_string(byte);
            }
            return result;
        default:
            result = std::string(data.begin(), data.end());
            return result;
        }
}

uint32_t Packet::extract_type(const char buffer[65535], size_t size) {
    std::tuple<unsigned, unsigned, unsigned, std::vector<uint8_t>> packet = extract_packet(buffer, size);

    return std::get<2>(packet);
}

uint32_t Packet::extract_id(const char buffer[65535], size_t size) {
    std::tuple<unsigned, unsigned, unsigned, std::vector<uint8_t>> packet = extract_packet(buffer, size);

    return std::get<1>(packet);
}

uint32_t Packet::extract_size(const char buffer[65535], size_t size) {
    std::tuple<unsigned, unsigned, unsigned, std::vector<uint8_t>> packet = extract_packet(buffer, size);

    return std::get<0>(packet);
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
    uint32_t size = 0;

    size += this->data_.size();
    switch (this->get_size_varint(size)) {
        case 1:
            this->size_ = static_cast<uint8_t>(size);
            break;
        case 2:
            this->size_ = static_cast<uint16_t>(size);
            break;
        case 3:
            this->size_ = convert_to_uint24(size);
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
    os << "Packet[" << packet.get_idp() << "]:" << std::endl;
    os << "   Size: " << packet.get_size() << std::endl;
    os << "   Type: " << static_cast<int>(packet.get_type()) << std::endl;
    os << "   Data: " << packet.get_data();
    return os;
}
