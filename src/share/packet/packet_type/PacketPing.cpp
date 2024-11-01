#include "PacketPing.hpp"

void PacketPing::format_data() {
    this->set_size();
    this->is_data_set_ = true;
}

