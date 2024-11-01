#include "PacketCMDP.hpp"

void PacketCMDP::format_data() {
    throw PacketException("No data provided for PacketCMDP", this->get_idp());
}

void PacketCMDP::format_data(uint8_t status) {
    this->data_.resize(1);
    this->data_[0] = status;
    this->set_size();
    this->is_data_set_ = true;
}
