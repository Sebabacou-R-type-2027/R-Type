#include "PacketACK.hpp"

void PacketACK::format_data(const bool status) {
    this->data_ = std::to_string(status);
    this->set_size();
    this->is_data_set_ = true;
}

void PacketACK::format_data() {
    throw Packet::PacketException("No data provided for PacketACK", this->get_idp());
}