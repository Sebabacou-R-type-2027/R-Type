#include "PacketACK.hpp"

void PacketACK::format_data() {
    throw PacketException("No data provided for PacketACK", this->get_idp());
}

void PacketACK::format_data(const bool status) {
    this->data_.resize(1);
    this->data_[0] = status;
    this->set_size();
    this->is_data_set_ = true;
}
