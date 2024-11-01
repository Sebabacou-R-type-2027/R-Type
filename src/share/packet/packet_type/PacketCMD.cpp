#include "PacketCMD.hpp"

void PacketCMD::format_data() {
    throw PacketException("No data provided for PacketCMD", this->get_idp());
}

void PacketCMD::format_data(std::string cmd) {
    //this->data_.resize(cmd.size());
    this->data_ = cmd;
    this->set_size();
    this->is_data_set_ = true;
}
