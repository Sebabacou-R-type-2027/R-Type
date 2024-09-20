#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include <iostream>
#include <asio.hpp>

#include "client.hpp"


using asio::ip::udp;

class UdpServer {
    public:
        UdpServer(asio::io_context& io_context, short port);

    private:
        void start_receive();
        void handle_receive(std::size_t bytes_transferred);
        void handle_new_connection(const udp::endpoint& client_endpoint, const std::string& username, const std::string& password);
        void handle_disconnect(const udp::endpoint& client_endpoint);

        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;
        std::vector<server::client> connected_clients_;
};

#endif //UDPSERVER_HPP

