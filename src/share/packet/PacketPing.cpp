#include "PacketPing.hpp"

void PacketPing::format_data() {
    this->data_ = std::to_string(this->get_idp());
    this->set_size();
    this->is_data_set_ = true;
}
