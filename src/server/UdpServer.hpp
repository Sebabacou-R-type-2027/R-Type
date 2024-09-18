//
// Created by shooting_star_t33 on 9/18/24.
//

#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include <iostream>
#include <asio.hpp>

using asio::ip::udp;

class UdpServer {
    public:
        UdpServer(asio::io_context& io_context, short port);
    private:
        void start_receive();

        void handle_receive(std::size_t bytes_transferred);

        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;
};

#endif //UDPSERVER_HPP
