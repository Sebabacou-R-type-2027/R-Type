#include "PacketLogout.hpp"

void PacketLogout::format_data(uint32_t id) {
    this->data_.resize(4);
    this->data_[0] = static_cast<uint8_t>((id >> 24) & 0xFF);
    this->data_[1] = static_cast<uint8_t>((id >> 16) & 0xFF);
    this->data_[2] = static_cast<uint8_t>((id >> 8) & 0xFF);
    this->data_[3] = static_cast<uint8_t>(id & 0xFF);
    this->set_size();
    this->is_data_set_ = true;
}


void PacketLogout::format_data() {
    throw Packet::PacketException("No data provided for PacketLogout", this->get_idp());
}
