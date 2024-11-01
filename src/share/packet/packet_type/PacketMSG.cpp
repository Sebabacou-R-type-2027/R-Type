#include "PacketMSG.hpp"

void PacketMSG::format_data() {
    throw PacketException("No data provided for PacketMSG", this->get_idp());
}

void PacketMSG::format_data(std::string cmd) {
    this->data_.resize(cmd.size());
    this->data_ = cmd;
    this->set_size();
    this->is_data_set_ = true;
}
