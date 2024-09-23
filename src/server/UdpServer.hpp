#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include <iostream>
#include <asio.hpp>
#include <vector>
#include <map>
#include "client.hpp"
#include "Lobby.hpp"

using asio::ip::udp;

class UdpServer {
    public:
        UdpServer(asio::io_context& io_context, short port);
        void handle_client_message(const std::string& message, const asio::ip::udp::endpoint& client_endpoint);
        client get_client(const asio::ip::udp::endpoint& client_endpoint);
    private:
        void start_receive();
        void handle_receive(std::size_t bytes_transferred);
        void handle_new_connection(const udp::endpoint& client_endpoint);
        void handle_disconnect(const udp::endpoint& client_endpoint);

        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;
        std::vector<client> connected_clients_;
};

#endif //UDPSERVER_HPP
