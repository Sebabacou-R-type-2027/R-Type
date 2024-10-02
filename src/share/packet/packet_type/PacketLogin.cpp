#include "PacketLogin.hpp"

void PacketLogin::format_data(const std::string username, const std::string password) {
    this->data_ = username + " " + password;
    this->set_size();
    this->is_data_set_ = true;
}

void PacketLogin::format_data() {
    throw Packet::PacketException("No data provided for PacketLogin", this->get_idp());
}