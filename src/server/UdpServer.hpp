#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include <asio.hpp>
#include <vector>
#include <map>
#include "client.hpp"
#include "Lobby.hpp"

class UdpServer {
    public:
        UdpServer(asio::io_context& io_context, short port);
        void handle_receive(std::size_t bytes_transferred);
        void handle_new_connection(const asio::ip::udp::endpoint& client_endpoint);
        void handle_client_message(const std::string& message, const asio::ip::udp::endpoint& client_endpoint);
        client get_client(const asio::ip::udp::endpoint& client_endpoint);
    private:
        void start_receive();
        void choose_host(const asio::ip::udp::endpoint& client_endpoint);

        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;
        std::vector<client> connected_clients_;
        std::map<int, Lobby> lobbys_;
};

#endif // UDPSERVER_HPP